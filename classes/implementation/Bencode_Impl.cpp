//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "Bencode.hpp"
#include "Bencode_Impl.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// NAMESPACE
// =========
namespace BencodeLib {
// ===========================
// PRIVATE TYPES AND CONSTANTS
// ===========================
// ==========================
// PUBLIC TYPES AND CONSTANTS
// ==========================
// ========================
// PRIVATE STATIC VARIABLES
// ========================
// =======================
// PUBLIC STATIC VARIABLES
// =======================
// ===============
// PRIVATE METHODS
// ===============
/// <summary>
/// Extract a Integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
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
/// <param name="source">Pointer to input interface used to decode Bencoded
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
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns>String BNode.</returns>
BNode::Ptr Bencode_Impl::decodeString(ISource &source) {
  return (makeString(extractString(source)));
}
/// <summary>
/// Decode an integer from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns>Integer BNode.</returns>
BNode::Ptr Bencode_Impl::decodeInteger(ISource &source) {
  source.next();
  int64_t integer = extractInteger(source);
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (makeInteger(integer));
}
/// <summary>
/// Decode a dictionary from the input stream of characters referenced by
/// ISource.
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns>Dictionary BNode.</returns>
BNode::Ptr Bencode_Impl::decodeDictionary(ISource &source) {
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
      dictionary.emplace_back(key, decodeBNodes(source));
    } else {
      throw Error("Syntax Error detected.");
    }
  }
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (makeDictionary(dictionary));
}
/// <summary>
/// Decode a list from the input stream of characters referenced by ISource.
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns>List BNode.</returns>
BNode::Ptr Bencode_Impl::decodeList(ISource &source) {
  std::vector<BNode::Ptr> list;
  source.next();
  while (source.more() && source.current() != 'e') {
    list.emplace_back(decodeBNodes(source));
  }
  if (source.current() != 'e') {
    throw Error("Syntax Error detected.");
  }
  source.next();
  return (makeList(list));
}
/// <summary>
/// Decode a BNode from the input stream of characters referenced by ISource.In
/// order to traverse
//  and decode complex encodings this method is called recursively to build up a
//  BNode structure.
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns>Root BNode.</returns>
BNode::Ptr Bencode_Impl::decodeBNodes(ISource &source) {
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
/// <summary>
/// Recursively traverse a BNode structure and produce an Bencode encoding of it
/// on the output stream referenced through the IDestination interface.
/// </summary>
/// <param name="bNode">Pointer to root of current BNode structure.</param>
/// <param name="destination ">Pointer to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode_Impl::encodeBNodes(const BNode &bNode, IDestination &destination) {
  switch (bNode.getNodeType()) {
  case BNodeType::dictionary:
    destination.add('d');
    for (const auto &bNodeEntry : BRef<Dictionary>(bNode).dictionary()) {
      destination.add(std::to_string(bNodeEntry.first.length()) + ":" +
                      bNodeEntry.first);
      encodeBNodes(*bNodeEntry.second, destination);
    }
    destination.add('e');
    break;
  case BNodeType::list:
    destination.add('l');
    for (const auto &bNodeEntry : BRef<List>(bNode).list()) {
      encodeBNodes(*bNodeEntry, destination);
    }
    destination.add('e');
    break;
  case BNodeType::integer:
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).integer()));
    destination.add('e');
    break;
  case BNodeType::string: {
    std::string stringToEncode = BRef<String>(bNode).string();
    destination.add(std::to_string(static_cast<int>(stringToEncode.length())) +
                    ":" + stringToEncode);
    break;
  }
  default:
    throw Error("Unknown BNode type encountered during encode.");
  }
}
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
///  Get JSONLib version.
/// </summary>
std::string Bencode_Impl::version() {
  std::stringstream versionString;
  versionString << "BencodeLib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return (versionString.str());
}
/// <summary>
/// Decode Bencoded byte string pointed to by source stream into BNode(s).
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param> <returns></returns>
void Bencode_Impl::decode(ISource &source) {
  m_bNodeRoot = decodeBNodes(source);
}
/// <summary>
/// Take BNode structure and create an Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination ">Pointer to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode_Impl::encode(IDestination &destination) {
  if (m_bNodeRoot == nullptr) {
    throw Error("No Bencoded data to encode.");
  }
  encodeBNodes(*m_bNodeRoot, destination);
}

std::unique_ptr<Variant> &BNode::getVariant() { return (m_bNodeVariant); }
const std::unique_ptr<Variant> &BNode::getVariant() const {
  return (m_bNodeVariant);
};
BNodeType BNode::getNodeType() const { return (m_bNodeVariant->getNodeType()); }
} // namespace BencodeLib
