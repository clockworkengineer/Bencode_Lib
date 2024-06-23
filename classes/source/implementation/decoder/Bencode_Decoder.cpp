#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Decoder.hpp"

namespace Bencode_Lib {

/// <summary>
/// Extract a Integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Positive integers value.</returns>
int64_t Bencode_Decoder::extractInteger(ISource &source) {
  // Number size of 64 bit int +2 for sign and terminating null
  std::array<char, std::numeric_limits<int64_t>::digits10 + 2> number{};
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
  // Check integer has no leading zero and is not empty ('ie')
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
/// Extract a byte string from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>String value decoded.</returns>
std::string Bencode_Decoder::extractString(ISource &source) {
  int64_t stringLength = extractInteger(source);
  if (source.current() != ':') {
    throw SyntaxError("Missing colon separator in string value.");
  }
  source.next();
  std::string buffer;
  while (stringLength-- > 0) {
    buffer += source.current();
    source.next();
  }
  return buffer;
}

/// <summary>
/// Decode a byte string from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>String BNode.</returns>
BNode Bencode_Decoder::decodeString(ISource &source) {
  return BNode::make<String>(extractString(source));
}

/// <summary>
/// Decode an integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Integer BNode.</returns>
BNode Bencode_Decoder::decodeInteger(ISource &source) {
  source.next();
  int64_t integer = extractInteger(source);
  if (source.current() != 'e') {
    throw SyntaxError("Missing end terminator on integer value.");
  }
  source.next();
  return BNode::make<Integer>(integer);
}

/// <summary>
/// Decode a dictionary from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Dictionary BNode.</returns>
BNode Bencode_Decoder::decodeDictionary(ISource &source) {
  BNode dictionary = BNode::make<Dictionary>();
  std::string lastKey{};
  source.next();
  while (source.more() && source.current() != 'e') {
    std::string key = extractString(source);
    // Check keys in lexical order
    if (lastKey > key) {
      throw SyntaxError("Dictionary keys not in sequence.");
    }
    lastKey = key;
    // Check key not duplicate and insert
    if (!BRef<Dictionary>(dictionary).contains(key)) {
      BRef<Dictionary>(dictionary).add(Dictionary::Entry(key, decodeTree(source)));
    } else {
      throw SyntaxError("Duplicate dictionary key.");
    }
  }
  if (source.current() != 'e') {
    throw SyntaxError("Missing end terminator on dictionary.");
  }
  source.next();
  return dictionary;
}

/// <summary>
/// Decode a list from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>List BNode.</returns>
BNode Bencode_Decoder::decodeList(ISource &source) {
  BNode list = BNode::make<List>();
  source.next();
  while (source.more() && source.current() != 'e') {
    BRef<List>(list).add(decodeTree(source));
  }
  if (source.current() != 'e') {
    throw SyntaxError("Missing end terminator on list.");
  }
  source.next();
  return list;
}

void Bencode_Decoder::confirmBoundary(ISource &source, char expectedBoundary) {
  if (source.current() != expectedBoundary) {
    throw SyntaxError(std::string("Missing end terminator on ") + expectedBoundary);
  }
  source.next();
}
/// <summary>
/// Decode a BNode tree root.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Root BNode.</returns>
BNode Bencode_Decoder::decodeTree(ISource &source) {
  using DecoderFunc = std::function<BNode(void)>;
  std::map<char, DecoderFunc> decoders = {
      {'d', [&source]{ return decodeDictionary(source); }},
      {'l', [&source]{ return decodeList(source); }},
      {'i', [&source]{ return decodeInteger(source); }},
      {'0', [&source]{ return decodeString(source); }},
      {'1', [&source]{ return decodeString(source); }},
      {'2', [&source]{ return decodeString(source); }},
      {'3', [&source]{ return decodeString(source); }},
      {'4', [&source]{ return decodeString(source); }},
      {'5', [&source]{ return decodeString(source); }},
      {'6', [&source]{ return decodeString(source); }},
      {'7', [&source]{ return decodeString(source); }},
      {'8', [&source]{ return decodeString(source); }},
      {'9', [&source]{ return decodeString(source); }}
  };

  auto it = decoders.find(source.current());
  if (it == decoders.end()) {
    throw SyntaxError("Expected integer, string, list or dictionary not present.");
  }
  return it->second();
}

/// <summary>
/// Decode a BNode from the input stream of characters referenced by ISource.In
/// order to traverse and decode complex encodings this method is called
/// recursively to build up a BNode structure.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Root BNode.</returns>
BNode Bencode_Decoder::decode(ISource &source) {
  return decodeTree(source);
}

} // namespace Bencode_Lib
