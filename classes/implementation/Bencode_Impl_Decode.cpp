//
// Class: Bencode_Impl_Decode
//
// Description: Bencode class implementation layer that uses recursion to
// produce a Bencoding tree (decode) and also reconstitute the tree back into
// raw Bencoding bytes (encode).
//
// Dependencies:   C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

/// <summary>
/// Extract a Integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Positive integers value.</returns>
int64_t Bencode_Impl::extractInteger(ISource &source) {
  // Number size of 64 bit int +2 for sign and terminating null
  std::array<char, std::numeric_limits<int64_t>::digits10 + 2> number{};
  std::size_t digits = 0;
  if (source.current() == '-') {
    number[digits++] = source.current();
    source.next();
  }
  while (source.more() && (std::isdigit(source.current()) != 0)) {
    // Number too large to be  in buffer
    if (digits == number.size()) {
      throw Error("Syntax Error detected.");
    }
    number[digits++] = source.current();
    source.next();
  }
  // Check integer has no leading zero and is not empty ('ie')
  if ((number[0] == '0' && digits > 1) || (digits == 0)) {
    throw Error("Syntax Error detected.");
  }
  // Check-for -0
  if ((number[0] == '-') && (number[1] == '0') && (digits == 2)) {
    throw Error("Syntax Error detected.");
  }
  return (std::stoll(&number[0]));
}

/// <summary>
/// Extract a byte string from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>String value decoded.</returns>
std::string Bencode_Impl::extractString(ISource &source) {
  int64_t stringLength = extractInteger(source);
  if (source.current() != ':') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  std::string buffer;
  while (stringLength-- > 0) {
    buffer += source.current();
    source.next();
  }
  return (buffer);
}

/// <summary>
/// Decode a byte string from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>String BNode.</returns>
BNode Bencode_Impl::decodeString(ISource &source) {
  return (BNode::make<String>(extractString(source)));
}

/// <summary>
/// Decode an integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Integer BNode.</returns>
BNode Bencode_Impl::decodeInteger(ISource &source) {
  source.next();
  int64_t integer = extractInteger(source);
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (BNode::make<Integer>(integer));
}

/// <summary>
/// Decode a dictionary from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Dictionary BNode.</returns>
BNode Bencode_Impl::decodeDictionary(ISource &source) {
  Dictionary::EntryList dictionary;
  std::string lastKey{};
  source.next();
  while (source.more() && source.current() != 'e') {
    std::string key = extractString(source);
    // Check keys in lexical order
    if (lastKey > key) {
      throw Error("Syntax Error detected.");
    }
    lastKey = key;
    // Check key not duplicate and insert
    if (std::find_if(dictionary.begin(), dictionary.end(),
                     [&key](const Dictionary::Entry &entry) -> bool {
                       return (entry.first == key);
                     }) == dictionary.end()) {
      dictionary.emplace_back(key, decodeBNode(source));
    } else {
      throw Error("Syntax Error detected.");
    }
  }
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (BNode::make<Dictionary>(dictionary));
}

/// <summary>
/// Decode a list from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>List BNode.</returns>
BNode Bencode_Impl::decodeList(ISource &source) {
  List::EntryList list;
  source.next();
  while (source.more() && source.current() != 'e') {
    list.emplace_back(decodeBNode(source));
  }
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (BNode::make<List>(list));
}

/// <summary>
/// Decode a BNode from the input stream of characters referenced by ISource.In
/// order to traverse
//  and decode complex encodings this method is called recursively to build up a
//  BNode structure.
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns>Root BNode.</returns>
BNode Bencode_Impl::decodeBNode(ISource &source) {
  switch (source.current()) {
  // Dictionary BNode
  case 'd':
    return (decodeDictionary(source));
  // List BNode
  case 'l':
    return (decodeList(source));
  // Integer BNode
  case 'i':
    return (decodeInteger(source));
  // String BNode
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return (decodeString(source));
  }
  throw Error("Syntax Error detected.");
}

} // namespace Bencode_Lib
