#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

/// <summary>
/// Extract an Integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns>Positive integers value.</returns>
Bencode::IntegerType Default_Parser::extractInteger(ISource &source) {
  // Number size of 64 bit int +2 for sign and terminating null
  std::array<char, std::numeric_limits<Bencode::IntegerType>::digits10 + 2> number{};
  std::size_t digits = 0;
  if (source.current() == '-') {
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
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="parserDepth">Current parser depth.</param>
///  <returns>String Node.</returns>
Node Default_Parser::parseString(ISource &source, [[maybe_unused]] const unsigned long parserDepth) {
  Bencode::IntegerType stringLength = extractInteger(source);
  if (stringLength < 0) {
    throw SyntaxError("Negative string length.");
  }
  if (source.current() != ':') {
    throw SyntaxError("Missing colon separator in string value.");
  }
  source.next();
  if (static_cast<uint64_t>(stringLength)>String::getMaxStringLength()) {
    throw SyntaxError("String size exceeds maximum allowed size.");
  }
  std::string buffer;
  while (stringLength-- > 0) {
    buffer += source.current();
    source.next();
  }
  return Node::make<String>(buffer);
}
/// <summary>
/// Parse an integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="parserDepth">Current parser depth.</param>
///  <returns>Integer Node.</returns>
Node Default_Parser::parseInteger(ISource &source, [[maybe_unused]]const unsigned long parserDepth) {
  source.next();
  Bencode::IntegerType integer = extractInteger(source);
  confirmBoundary(source, 'e');
  return Node::make<Integer>(integer);
}
/// <summary>
/// Parse a dictionary from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Dictionary Node.</returns>
Node Default_Parser::parseDictionary(ISource &source, const unsigned long parserDepth) {
  Node dictionary = Node::make<Dictionary>();
  std::string lastKey{};
  source.next();
  while (source.more() && source.current() != 'e') {
    const std::string_view key = { NRef<String>(parseString(source,parserDepth)).value()} ;
    // Check keys in lexical order
    if (lastKey > key) {
      throw SyntaxError("Dictionary keys not in sequence.");
    }
    lastKey = key;
    // Check key not duplicate and insert
    if (!NRef<Dictionary>(dictionary).contains(key)) {
      NRef<Dictionary>(dictionary)
          .add(Dictionary::Entry(key, parseNodes(source, parserDepth+1)));
    } else {
      throw SyntaxError("Duplicate dictionary key.");
    }
  }
  confirmBoundary(source, 'e');
  return dictionary;
}
/// <summary>
/// Parse a list from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>List Node.</returns>
Node Default_Parser::parseList(ISource &source, const unsigned long parserDepth) {
  Node list = Node::make<List>();
  source.next();
  while (source.more() && source.current() != 'e') {
    NRef<List>(list).add(parseNodes(source, parserDepth+1));
  }
  confirmBoundary(source, 'e');
  return list;
}
/// <summary>
/// Generate SyntaxError if the expected boundary character is not found.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="expectedBoundary">Expected boundary character.</param>
void Default_Parser::confirmBoundary(ISource &source, const char expectedBoundary) {
  if (source.current() != expectedBoundary) {
    throw SyntaxError(std::string("Missing end terminator on ") +
                      expectedBoundary);
  }
  source.next();
}
/// <summary>
/// Parse a Node tree root.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded stream.</param>
/// <param name="parserDepth">Current parser depth.</param>
/// <returns>Root Node.</returns>
Node Default_Parser::parseNodes(ISource &source, const unsigned long parserDepth) {
  if (parserDepth>=getMaxParserDepth()) {
    throw SyntaxError("Maximum parser depth exceeded.");
  }
  const auto it = parsers.find(source.current());
  if (it == parsers.end()) {
    throw SyntaxError(
        "Expected integer, string, list or dictionary not present.");
  }
  return it->second(source, parserDepth);
}
/// <summary>
/// Parse a Node from the input stream of characters referenced by ISource to
/// traverse and parse complex encodings. This method is called
/// recursively to build up a Node structure.
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns>Root Node.</returns>
Node Default_Parser::parse(ISource &source) { return parseNodes(source, 1); }
} // namespace Bencode_Lib
