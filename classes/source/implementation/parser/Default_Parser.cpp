#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Parser_Constants.hpp"

namespace Bencode_Lib {

#if defined(BENCODE_ENABLE_EXCEPTIONS)

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
  return std::stoll(&number[0]);
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
    return Node::make<String>(std::string{});
  }
  std::string buffer;
  buffer.resize(static_cast<std::size_t>(stringLength));
  for (std::size_t index = 0; index < static_cast<std::size_t>(stringLength);
       ++index) {
    buffer[index] = source.current();
    source.next();
  }
  return Node::make<String>(std::move(buffer));
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
Node Default_Parser::parse(ISource &source) { return parseNodes(source, 1); }

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
  try {
    number = std::stoll(&buffer[0]);
  } catch (...) {
    return makeSyntaxError("Integer conversion failed.");
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
    destination = Node::make<String>(std::string{});
    return ParseStatus::success();
  }
  std::string buffer;
  buffer.resize(static_cast<std::size_t>(stringLength));
  for (std::size_t index = 0; index < static_cast<std::size_t>(stringLength);
       ++index) {
    buffer[index] = source.current();
    source.next();
  }
  destination = Node::make<String>(std::move(buffer));
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
  return parseNodes(source, 1, destination);
}

#endif

} // namespace Bencode_Lib
