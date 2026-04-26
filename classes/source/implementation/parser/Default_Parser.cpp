#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Parser_Constants.hpp"

#include <vector>

namespace Bencode_Lib {

enum class ContainerType { List, Dictionary };

struct ParserFrame {
  ContainerType type;
  Node container;
  std::string lastKey;
  std::string currentKey;
  bool awaitingValue = false;

  explicit ParserFrame(ContainerType frameType)
      : type(frameType),
        container(frameType == ContainerType::List ? Node::make<List>()
                                                   : Node::make<Dictionary>()),
        lastKey(), currentKey(), awaitingValue(false) {}
};

static bool convertToInteger(const char *buffer, const std::size_t digits,
                             Bencode::IntegerType &value) {
  const bool negative =
      digits > 0 && buffer[0] == ParserConstants::STRING_MINUS;
  std::size_t start = negative ? 1 : 0;
  unsigned long long limit = static_cast<unsigned long long>(
      std::numeric_limits<Bencode::IntegerType>::max());
  if (negative) {
    limit += 1ull;
  }
  unsigned long long result = 0ull;
  for (std::size_t index = start; index < digits; ++index) {
    unsigned int digit = static_cast<unsigned int>(buffer[index] - '0');
    if (result > (limit - digit) / 10ull) {
      return false;
    }
    result = (result * 10ull) + digit;
  }
  if (negative) {
    value = -static_cast<Bencode::IntegerType>(result);
  } else {
    value = static_cast<Bencode::IntegerType>(result);
  }
  return true;
}

#if BENCODE_ENABLE_EXCEPTIONS

/// <summary>
/// Extract an Integer from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns>Positive integers value.</returns>
Bencode::IntegerType Default_Parser::extractInteger(ISource &source) {
  // Number size of 64 bit int +2 for sign and terminating null
  std::array<char, std::numeric_limits<Bencode::IntegerType>::digits10 + 2>
      number{};
  std::size_t digits = 0;
  if (source.current() == ParserConstants::STRING_MINUS) {
    number[digits++] = source.current();
    source.next();
  }
  while (source.more() && std::isdigit(source.current()) != 0) {
    // Number too large to fit in buffer
    if (digits == number.size()) {
      throw SyntaxError("Integer to large to fit in conversion buffer.");
    }
    number[digits++] = source.current();
    source.next();
  }
  // Check integer has no leading zero and is not empty ('i.e.')
  if ((number[0] == '0' && digits > 1) || digits == 0) {
    throw SyntaxError("Empty Integer or has leading zero.");
  }
  // Check-for -0
  if (number[0] == '-' && number[1] == '0' && digits == 2) {
    throw SyntaxError("Negative zero is not allowed.");
  }
  Bencode::IntegerType integer = 0;
  if (!convertToInteger(number.data(), digits, integer)) {
    throw std::out_of_range("Integer conversion overflow.");
  }
  return integer;
}
/// <summary>
/// Parse a byte string from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="parserDepth">Current parser depth.</param>
///  <returns>String Node.</returns>
Node Default_Parser::parseString(
    ISource &source, [[maybe_unused]] const unsigned long parserDepth) {
  Bencode::IntegerType stringLength = extractInteger(source);
  if (stringLength < 0) {
    throw SyntaxError("Negative string length.");
  }
  if (source.current() != ParserConstants::COLON) {
    throw SyntaxError("Missing colon separator in string value.");
  }
  source.next();
  if (static_cast<uint64_t>(stringLength) > String::getMaxStringLength()) {
    throw SyntaxError("String size exceeds maximum allowed size.");
  }
  if (stringLength == 0) {
    return Node::make<String>();
  }
  Node result = Node::make<String>(static_cast<std::size_t>(stringLength));
  char *payload = NRef<String>(result).data();
  for (std::size_t index = 0;
       index < static_cast<std::size_t>(stringLength); ++index) {
    payload[index] = source.current();
    source.next();
  }
  return result;
}
/// <summary>
/// Parse an integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="parserDepth">Current parser depth.</param>
///  <returns>Integer Node.</returns>
Node Default_Parser::parseInteger(
    ISource &source, [[maybe_unused]] const unsigned long parserDepth) {
  source.next();
  Bencode::IntegerType integer = extractInteger(source);
  confirmBoundary(source, ParserConstants::END);
  return Node::make<Integer>(integer);
}
/// <summary>
/// Parse a dictionary from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="parserDepth">Current parser depth.</param>
/// <returns>Dictionary Node.</returns>
Node Default_Parser::parseDictionary(ISource &source,
                                     const unsigned long parserDepth) {
  Node dictionary = Node::make<Dictionary>();
  std::string lastKey{};
  source.next();
  while (source.more() && source.current() != ParserConstants::END) {
    Node keyNode = parseString(source, parserDepth);
    std::string key = std::string(NRef<String>(keyNode).value());
    // Check keys in lexical order
    if (lastKey > key) {
      throw SyntaxError("Dictionary keys not in sequence.");
    }
    lastKey = key;
    // Check key not duplicate and insert
    if (!NRef<Dictionary>(dictionary).contains(key)) {
      NRef<Dictionary>(dictionary)
          .add(Dictionary::Entry(key, parseNodes(source, parserDepth + 1)));
    } else {
      throw SyntaxError("Duplicate dictionary key.");
    }
  }
  confirmBoundary(source, ParserConstants::END);
  return dictionary;
}
/// <summary>
/// Parse a list from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="parserDepth">Current parser depth.</param>
/// <returns>List Node.</returns>
Node Default_Parser::parseList(ISource &source,
                               const unsigned long parserDepth) {
  Node list = Node::make<List>();
  source.next();
  while (source.more() && source.current() != ParserConstants::END) {
    NRef<List>(list).add(parseNodes(source, parserDepth + 1));
  }
  confirmBoundary(source, ParserConstants::END);
  return list;
}

Node Default_Parser::parseScalar(ISource &source,
                                 const unsigned long parserDepth) {
  switch (source.current()) {
  case ParserConstants::INTEGER:
    return Default_Parser::parseInteger(source, parserDepth);
  case ParserConstants::STRING_0:
  case ParserConstants::STRING_1:
  case ParserConstants::STRING_2:
  case ParserConstants::STRING_3:
  case ParserConstants::STRING_4:
  case ParserConstants::STRING_5:
  case ParserConstants::STRING_6:
  case ParserConstants::STRING_7:
  case ParserConstants::STRING_8:
  case ParserConstants::STRING_9:
  case ParserConstants::STRING_MINUS:
  case ParserConstants::STRING_PLUS:
    return Default_Parser::parseString(source, parserDepth);
  default:
    throw SyntaxError("Expected integer or string while parsing container.");
  }
}

Node Default_Parser::parseIterative(ISource &source) {
  if (!source.more()) {
    throw SyntaxError("Unexpected end of source.");
  }

  std::vector<ParserFrame> frameStack;
  auto pushFrame = [&](ContainerType type) {
    if (frameStack.size() + 1 >= getMaxParserDepth()) {
      throw SyntaxError("Maximum parser depth exceeded.");
    }
    frameStack.emplace_back(type);
  };

  auto completeFrame = [&]() -> Node {
    Node completed = std::move(frameStack.back().container);
    frameStack.pop_back();
    if (frameStack.empty()) {
      return completed;
    }
    ParserFrame &parent = frameStack.back();
    if (parent.type == ContainerType::List) {
      NRef<List>(parent.container).add(std::move(completed));
    } else {
      if (!parent.awaitingValue) {
        throw SyntaxError("Dictionary value missing key.");
      }
      NRef<Dictionary>(parent.container)
          .add(Dictionary::Entry(std::move(parent.currentKey),
                                 std::move(completed)));
      parent.awaitingValue = false;
    }
    return Node{};
  };

  if (source.current() == ParserConstants::DICTIONARY) {
    source.next();
    pushFrame(ContainerType::Dictionary);
  } else if (source.current() == ParserConstants::LIST) {
    source.next();
    pushFrame(ContainerType::List);
  } else {
    return parseNodes(source, 1);
  }

  Node root;
  while (!frameStack.empty()) {
    ParserFrame &frame = frameStack.back();
    if (frame.type == ContainerType::List) {
      if (!source.more()) {
        throw SyntaxError("Unexpected end of source.");
      }
      if (source.current() == ParserConstants::END) {
        source.next();
        root = completeFrame();
        continue;
      }
      if (source.current() == ParserConstants::DICTIONARY) {
        source.next();
        pushFrame(ContainerType::Dictionary);
        continue;
      }
      if (source.current() == ParserConstants::LIST) {
        source.next();
        pushFrame(ContainerType::List);
        continue;
      }
      Node child = parseScalar(
          source, static_cast<unsigned long>(frameStack.size() + 1));
      NRef<List>(frame.container).add(std::move(child));
      continue;
    }

    if (frame.awaitingValue) {
      if (!source.more()) {
        throw SyntaxError("Unexpected end of source.");
      }
      if (source.current() == ParserConstants::DICTIONARY) {
        source.next();
        pushFrame(ContainerType::Dictionary);
        continue;
      }
      if (source.current() == ParserConstants::LIST) {
        source.next();
        pushFrame(ContainerType::List);
        continue;
      }
      Node valueNode = parseScalar(
          source, static_cast<unsigned long>(frameStack.size() + 1));
      NRef<Dictionary>(frame.container)
          .add(Dictionary::Entry(std::move(frame.currentKey),
                                 std::move(valueNode)));
      frame.awaitingValue = false;
      continue;
    }

    if (!source.more()) {
      throw SyntaxError("Unexpected end of source.");
    }
    if (source.current() == ParserConstants::END) {
      source.next();
      root = completeFrame();
      continue;
    }
    Node keyNode =
        parseString(source, static_cast<unsigned long>(frameStack.size() + 1));
    std::string key = std::string(NRef<String>(keyNode).value());
    if (frame.lastKey > key) {
      throw SyntaxError("Dictionary keys not in sequence.");
    }
    frame.lastKey = key;
    if (NRef<Dictionary>(frame.container).contains(key)) {
      throw SyntaxError("Duplicate dictionary key.");
    }
    frame.currentKey = std::move(key);
    frame.awaitingValue = true;
  }

  return root;
}
/// <summary>
/// Generate SyntaxError if the expected boundary character is not found.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="expectedBoundary">Expected boundary
/// character.</param>
void Default_Parser::confirmBoundary(ISource &source,
                                     const char expectedBoundary) {
  if (source.current() != expectedBoundary) {
    throw SyntaxError(std::string("Missing end terminator on ") +
                      expectedBoundary);
  }
  source.next();
}
/// <summary>
/// Parse a Node tree root.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <param name="parserDepth">Current parser depth.</param>
/// <returns>Root Node.</returns>
Node Default_Parser::parseNodes(ISource &source,
                                const unsigned long parserDepth) {
  if (parserDepth >= getMaxParserDepth()) {
    throw SyntaxError("Maximum parser depth exceeded.");
  }

  switch (source.current()) {
  case ParserConstants::DICTIONARY:
    return parseDictionary(source, parserDepth);
  case ParserConstants::LIST:
    return parseList(source, parserDepth);
  case ParserConstants::INTEGER:
    return parseInteger(source, parserDepth);
  case ParserConstants::STRING_0:
  case ParserConstants::STRING_1:
  case ParserConstants::STRING_2:
  case ParserConstants::STRING_3:
  case ParserConstants::STRING_4:
  case ParserConstants::STRING_5:
  case ParserConstants::STRING_6:
  case ParserConstants::STRING_7:
  case ParserConstants::STRING_8:
  case ParserConstants::STRING_9:
  case ParserConstants::STRING_MINUS:
  case ParserConstants::STRING_PLUS:
    return parseString(source, parserDepth);
  default:
    throw SyntaxError(
        "Expected integer, string, list or dictionary not present.");
  }
}
/// <summary>
/// Parse a Node from the input stream of characters referenced by ISource to
/// traverse and parse complex encodings. This method is called
/// recursively to build up a Node structure.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns>Root Node.</returns>
Node Default_Parser::parse(ISource &source) { return parseIterative(source); }

#else

static ParseStatus makeSyntaxError(const std::string_view &message) {
  return ParseStatus::failure(ErrorCode::SyntaxError, std::string(message));
}

ParseStatus Default_Parser::extractInteger(ISource &source,
                                           Bencode::IntegerType &number) {
  std::array<char, std::numeric_limits<Bencode::IntegerType>::digits10 + 2>
      buffer{};
  std::size_t digits = 0;
  if (source.current() == ParserConstants::STRING_MINUS) {
    buffer[digits++] = source.current();
    source.next();
  }
  while (source.more() && std::isdigit(source.current()) != 0) {
    if (digits == buffer.size()) {
      return makeSyntaxError("Integer to large to fit in conversion buffer.");
    }
    buffer[digits++] = source.current();
    source.next();
  }
  if ((buffer[0] == '0' && digits > 1) || digits == 0) {
    return makeSyntaxError("Empty Integer or has leading zero.");
  }
  if (buffer[0] == '-' && buffer[1] == '0' && digits == 2) {
    return makeSyntaxError("Negative zero is not allowed.");
  }
  if (!convertToInteger(buffer.data(), digits, number)) {
    return ParseStatus::failure(ErrorCode::IntegerOverflow,
                                "Integer conversion overflow.");
  }
  return ParseStatus::success();
}

ParseStatus
Default_Parser::parseString(ISource &source,
                            [[maybe_unused]] const unsigned long parserDepth,
                            Node &destination) {
  Bencode::IntegerType stringLength = 0;
  ParseStatus status = extractInteger(source, stringLength);
  if (!status.ok()) {
    return status;
  }
  if (stringLength < 0) {
    return makeSyntaxError("Negative string length.");
  }
  if (source.current() != ParserConstants::COLON) {
    return makeSyntaxError("Missing colon separator in string value.");
  }
  source.next();
  if (static_cast<uint64_t>(stringLength) > String::getMaxStringLength()) {
    return makeSyntaxError("String size exceeds maximum allowed size.");
  }
  if (stringLength == 0) {
    destination = Node::make<String>();
    return ParseStatus::success();
  }
  destination = Node::make<String>(static_cast<std::size_t>(stringLength));
  char *payload = NRef<String>(destination).data();
  for (std::size_t index = 0;
       index < static_cast<std::size_t>(stringLength); ++index) {
    payload[index] = source.current();
    source.next();
  }
  return ParseStatus::success();
}

ParseStatus
Default_Parser::parseInteger(ISource &source,
                             [[maybe_unused]] const unsigned long parserDepth,
                             Node &destination) {
  source.next();
  Bencode::IntegerType integer = 0;
  ParseStatus status = extractInteger(source, integer);
  if (!status.ok()) {
    return status;
  }
  status = confirmBoundary(source, ParserConstants::END);
  if (!status.ok()) {
    return status;
  }
  destination = Node::make<Integer>(integer);
  return ParseStatus::success();
}

ParseStatus Default_Parser::parseDictionary(ISource &source,
                                            const unsigned long parserDepth,
                                            Node &destination) {
  Node dictionary = Node::make<Dictionary>();
  std::string lastKey{};
  source.next();
  while (source.more() && source.current() != ParserConstants::END) {
    Node keyNode;
    ParseStatus status = parseString(source, parserDepth, keyNode);
    if (!status.ok()) {
      return status;
    }
    std::string key = std::string(NRef<String>(keyNode).value());
    if (lastKey > key) {
      return makeSyntaxError("Dictionary keys not in sequence.");
    }
    lastKey = key;
    if (!NRef<Dictionary>(dictionary).contains(key)) {
      Node valueNode;
      status = parseNodes(source, parserDepth + 1, valueNode);
      if (!status.ok()) {
        return status;
      }
      NRef<Dictionary>(dictionary)
          .add(Dictionary::Entry(key, std::move(valueNode)));
    } else {
      return makeSyntaxError("Duplicate dictionary key.");
    }
  }
  ParseStatus boundaryStatus = confirmBoundary(source, ParserConstants::END);
  if (!boundaryStatus.ok()) {
    return boundaryStatus;
  }
  destination = std::move(dictionary);
  return ParseStatus::success();
}

ParseStatus Default_Parser::parseList(ISource &source,
                                      const unsigned long parserDepth,
                                      Node &destination) {
  Node list = Node::make<List>();
  source.next();
  while (source.more() && source.current() != ParserConstants::END) {
    Node valueNode;
    ParseStatus status = parseNodes(source, parserDepth + 1, valueNode);
    if (!status.ok()) {
      return status;
    }
    NRef<List>(list).add(std::move(valueNode));
  }
  ParseStatus boundaryStatus = confirmBoundary(source, ParserConstants::END);
  if (!boundaryStatus.ok()) {
    return boundaryStatus;
  }
  destination = std::move(list);
  return ParseStatus::success();
}

ParseStatus Default_Parser::parseScalar(ISource &source,
                                        const unsigned long parserDepth,
                                        Node &destination) {
  switch (source.current()) {
  case ParserConstants::INTEGER:
    return Default_Parser::parseInteger(source, parserDepth, destination);
  case ParserConstants::STRING_0:
  case ParserConstants::STRING_1:
  case ParserConstants::STRING_2:
  case ParserConstants::STRING_3:
  case ParserConstants::STRING_4:
  case ParserConstants::STRING_5:
  case ParserConstants::STRING_6:
  case ParserConstants::STRING_7:
  case ParserConstants::STRING_8:
  case ParserConstants::STRING_9:
  case ParserConstants::STRING_MINUS:
  case ParserConstants::STRING_PLUS:
    return Default_Parser::parseString(source, parserDepth, destination);
  default:
    return makeSyntaxError(
        "Expected integer or string while parsing container.");
  }
}

static ParseStatus attachCompletedValue(ParserFrame &parent, Node &&completed) {
  if (parent.type == ContainerType::List) {
    NRef<List>(parent.container).add(std::move(completed));
    return ParseStatus::success();
  }
  if (!parent.awaitingValue) {
    return makeSyntaxError("Dictionary value missing key.");
  }
  NRef<Dictionary>(parent.container)
      .add(Dictionary::Entry(std::move(parent.currentKey),
                             std::move(completed)));
  parent.awaitingValue = false;
  return ParseStatus::success();
}

ParseStatus Default_Parser::parseIterative(ISource &source, Node &destination) {
  if (!source.more()) {
    return makeSyntaxError("Unexpected end of source.");
  }

  std::vector<ParserFrame> frameStack;
  auto pushFrame = [&](ContainerType type) -> ParseStatus {
    if (frameStack.size() + 1 >= getMaxParserDepth()) {
      return makeSyntaxError("Maximum parser depth exceeded.");
    }
    frameStack.emplace_back(type);
    return ParseStatus::success();
  };

  auto completeFrame = [&](Node &&completed) -> ParseStatus {
    if (frameStack.empty()) {
      destination = std::move(completed);
      return ParseStatus::success();
    }
    ParserFrame &parent = frameStack.back();
    return attachCompletedValue(parent, std::move(completed));
  };

  if (source.current() == ParserConstants::DICTIONARY) {
    source.next();
    ParseStatus status = pushFrame(ContainerType::Dictionary);
    if (!status.ok()) {
      return status;
    }
  } else if (source.current() == ParserConstants::LIST) {
    source.next();
    ParseStatus status = pushFrame(ContainerType::List);
    if (!status.ok()) {
      return status;
    }
  } else {
    return parseNodes(source, 1, destination);
  }

  while (!frameStack.empty()) {
    ParserFrame &frame = frameStack.back();
    if (frame.type == ContainerType::List) {
      if (!source.more()) {
        return makeSyntaxError("Unexpected end of source.");
      }
      if (source.current() == ParserConstants::END) {
        source.next();
        Node completed = std::move(frame.container);
        frameStack.pop_back();
        if (frameStack.empty()) {
          destination = std::move(completed);
          return ParseStatus::success();
        }
        ParseStatus status =
            attachCompletedValue(frameStack.back(), std::move(completed));
        if (!status.ok()) {
          return status;
        }
        continue;
      }
      if (source.current() == ParserConstants::DICTIONARY) {
        source.next();
        ParseStatus status = pushFrame(ContainerType::Dictionary);
        if (!status.ok()) {
          return status;
        }
        continue;
      }
      if (source.current() == ParserConstants::LIST) {
        source.next();
        ParseStatus status = pushFrame(ContainerType::List);
        if (!status.ok()) {
          return status;
        }
        continue;
      }
      Node valueNode;
      ParseStatus status = parseScalar(
          source, static_cast<unsigned long>(frameStack.size() + 1), valueNode);
      if (!status.ok()) {
        return status;
      }
      NRef<List>(frame.container).add(std::move(valueNode));
      continue;
    }

    if (frame.awaitingValue) {
      if (!source.more()) {
        return makeSyntaxError("Unexpected end of source.");
      }
      if (source.current() == ParserConstants::DICTIONARY) {
        source.next();
        ParseStatus status = pushFrame(ContainerType::Dictionary);
        if (!status.ok()) {
          return status;
        }
        continue;
      }
      if (source.current() == ParserConstants::LIST) {
        source.next();
        ParseStatus status = pushFrame(ContainerType::List);
        if (!status.ok()) {
          return status;
        }
        continue;
      }
      Node valueNode;
      ParseStatus status = parseScalar(
          source, static_cast<unsigned long>(frameStack.size() + 1), valueNode);
      if (!status.ok()) {
        return status;
      }
      NRef<Dictionary>(frame.container)
          .add(Dictionary::Entry(std::move(frame.currentKey),
                                 std::move(valueNode)));
      frame.awaitingValue = false;
      continue;
    }

    if (!source.more()) {
      return makeSyntaxError("Unexpected end of source.");
    }
    if (source.current() == ParserConstants::END) {
      source.next();
      Node completed = std::move(frame.container);
      frameStack.pop_back();
      if (frameStack.empty()) {
        destination = std::move(completed);
        return ParseStatus::success();
      }
      ParseStatus status =
          attachCompletedValue(frameStack.back(), std::move(completed));
      if (!status.ok()) {
        return status;
      }
      continue;
    }
    Node keyNode;
    ParseStatus status = parseString(
        source, static_cast<unsigned long>(frameStack.size() + 1), keyNode);
    if (!status.ok()) {
      return status;
    }
    std::string key = std::string(NRef<String>(keyNode).value());
    if (frame.lastKey > key) {
      return makeSyntaxError("Dictionary keys not in sequence.");
    }
    frame.lastKey = key;
    if (NRef<Dictionary>(frame.container).contains(key)) {
      return makeSyntaxError("Duplicate dictionary key.");
    }
    frame.currentKey = std::move(key);
    frame.awaitingValue = true;
  }

  return ParseStatus::success();
}

ParseStatus Default_Parser::confirmBoundary(ISource &source,
                                            const char expectedBoundary) {
  if (source.current() != expectedBoundary) {
    return ParseStatus::failure(ErrorCode::MissingEndTerminator,
                                std::string("Missing end terminator on ") +
                                    expectedBoundary);
  }
  source.next();
  return ParseStatus::success();
}

ParseStatus Default_Parser::parseNodes(ISource &source,
                                       const unsigned long parserDepth,
                                       Node &destination) {
  if (parserDepth >= getMaxParserDepth()) {
    return makeSyntaxError("Maximum parser depth exceeded.");
  }

  switch (source.current()) {
  case ParserConstants::DICTIONARY:
    return parseDictionary(source, parserDepth, destination);
  case ParserConstants::LIST:
    return parseList(source, parserDepth, destination);
  case ParserConstants::INTEGER:
    return parseInteger(source, parserDepth, destination);
  case ParserConstants::STRING_0:
  case ParserConstants::STRING_1:
  case ParserConstants::STRING_2:
  case ParserConstants::STRING_3:
  case ParserConstants::STRING_4:
  case ParserConstants::STRING_5:
  case ParserConstants::STRING_6:
  case ParserConstants::STRING_7:
  case ParserConstants::STRING_8:
  case ParserConstants::STRING_9:
  case ParserConstants::STRING_MINUS:
  case ParserConstants::STRING_PLUS:
    return parseString(source, parserDepth, destination);
  default:
    return makeSyntaxError(
        "Expected integer, string, list or dictionary not present.");
  }
}

ParseStatus Default_Parser::parse(ISource &source, Node &destination) {
  return parseIterative(source, destination);
}

#endif

} // namespace Bencode_Lib
