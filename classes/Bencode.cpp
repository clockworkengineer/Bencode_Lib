//
// Class: Bencode
//
// Description: Perform Bencode encoding encode/decode to/from a byte
// buffer or file. It is also  possible to customize this with the
// ISource and IDestination interfaces if required. Although Bencoded
// data is treated as std::byte externally this library uses char and
// std::string internally.Note: At present it will report incorrect Bencode
// syntax but will not be specific about what error has occurred; this
// is reasoned to add too much overhead to the process of parsing for the
// requirements of this library (this might change in future versions).
// For an in-depth description of the Bencode specification refer to its
// Wiki page at https://en.wikipedia.org/wiki/Bencode.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

/// <summary>
/// Initialise the implementation layer.
/// </summary>

Bencode::Bencode(IEncoder *encoder, IDecoder *decoder)
    : implementation(std::make_unique<Bencode_Impl>()) {
  implementation->setEncoder(encoder);
  implementation->setDecoder(decoder);
}
Bencode::~Bencode() {}

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
/// <param name="list">Intialiser list of single values or JNode.</param>
Bencode::Bencode(const ListInitializer &list) : Bencode() { this->root() = BNode(list); }

/// <summary>
/// Bencode constructor (dictionary).
/// </summary>
/// <param name="object">Intialiser list of key/value(JNode) pairs.</param>
Bencode::Bencode(const DictionaryInitializer &dictionary) : Bencode() { this->root() = BNode(dictionary); }

/// <summary>
///  Get Bencode_Lib version.
/// </summary>
std::string Bencode::version() const { return (implementation->version()); }

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
/// <param name="destination ">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode::encode(IDestination &destination) const {
  implementation->encode(destination);
}
void Bencode::encode(IDestination &&destination) const {
  implementation->encode(destination);
}

/// <summary>
/// Get root of BNode tree.
/// </summary>
/// <returns>Root of BNode encoded tree.</returns>
BNode &Bencode::root() { return (implementation->root()); }
const BNode &Bencode::root() const { return (implementation->root()); }

/// <summary>
/// Return object entry for the passed in key.
/// </summary>
/// <param name=key>Dictionary entry (BNode) key.</param>
BNode &Bencode::operator[](const std::string &key) {
  return ((*implementation)[key]);
}
const BNode &Bencode::operator[](const std::string &key) const {
  return ((*implementation)[key]);
}

/// <summary>
/// Return list entry for the passed in index.
/// </summary>
/// <param name=index>Array entry (BNode) index.</param>
BNode &Bencode::operator[](std::size_t index) {
  return ((*implementation)[index]);
}
const BNode &Bencode::operator[](std::size_t index) const {
  return ((*implementation)[index]);
}

} // namespace Bencode_Lib
