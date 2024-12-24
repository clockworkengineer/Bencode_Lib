//
// Class: Bencode
//
// Description: Perform Bencode encode/decode to/from a byte
// buffer or file.It is also possible to customize this with the
// ISource and IDestination interfaces if required. Although Bencoded
// data is treated as std::byte externally, this library uses char and
// std::string internally.
//
// For an in-depth description of the Bencode specification refer to its
// Wiki page at https://en.wikipedia.org/wiki/Bencode.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

/// <summary>
/// Initialise the implementation layer.
/// </summary>

Bencode::Bencode([[maybe_unused]] IEncoder *encoder,
                 [[maybe_unused]] IDecoder *decoder)
    : implementation(std::make_unique<Bencode_Impl>(encoder, decoder)) {}
Bencode::~Bencode() = default;

/// <summary>
/// Bencode constructor. Pass a Bencode string to be initially parsed.
/// </summary>
/// <param name="bencodeString">Bencode string.</param>
Bencode::Bencode(const std::string &bencodeString) : Bencode() {
  decode(BufferSource{bencodeString});
}

/// <summary>
/// Bencode constructor (list).
/// </summary>
/// <param name="list">Initializer list of single values or JNode.</param>
Bencode::Bencode(const ListInitializer &list) : Bencode() {
  this->root() = BNode(list);
}

/// <summary>
/// Bencode constructor (dictionary).
/// </summary>
/// <param name="dictionary">Initializer list of key/value(JNode) pairs.</param>
Bencode::Bencode(const DictionaryInitializer &dictionary) : Bencode() {
  this->root() = BNode(dictionary);
}

/// <summary>
///  Get Bencode_Lib version.
/// </summary>
std::string Bencode::version() const { return implementation->version(); }

/// <summary>
/// Decode Bencoded byte string pointed to by source stream into BNode(s).
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns></returns>
void Bencode::decode(ISource &source) const { implementation->decode(source); }
void Bencode::decode(ISource &&source) const { implementation->decode(source); }

/// <summary>
/// Take BNode structure and create an Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode::encode(IDestination &destination) const {
  implementation->encode(destination);
}
void Bencode::encode(IDestination &&destination) const {
  implementation->encode(destination);
}

/// <summary>
/// Recursively traverse JNode structure calling IAction methods (read only)
///  or to change the Bencode tree node directly.
/// </summary>
/// <param name="action">Action methods to call during traversal.</param>
/// Traverse using non-const Bencode so can change Bencode tree
[[maybe_unused]] void Bencode::traverse(IAction &action) { implementation->traverse(action); }
// Traverse using const Bencode so cannot change the Bencode tree
void Bencode::traverse(IAction &action) const {
  std::as_const(*implementation).traverse(action);
}

/// <summary>
/// Get root of BNode tree.
/// </summary>
/// <returns>Root of BNode encoded tree.</returns>
BNode &Bencode::root() { return implementation->root(); }
const BNode &Bencode::root() const { return implementation->root(); }

/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name="key">Dictionary entry (BNode) key.</param>
BNode &Bencode::operator[](const std::string &key) {
  return (*implementation)[key];
}
const BNode &Bencode::operator[](const std::string &key) const {
  return (*implementation)[key];
}

/// <summary>
/// Return list entry for the passed in index.
/// </summary>
/// <param name="index">Array entry (BNode) index.</param>
BNode &Bencode::operator[](const std::size_t index) {
  return (*implementation)[index];
}
const BNode &Bencode::operator[](const std::size_t index) const {
  return (*implementation)[index];
}

} // namespace Bencode_Lib
