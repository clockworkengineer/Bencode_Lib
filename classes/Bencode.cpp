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

Bencode::Bencode() : implementation(std::make_unique<Bencode_Impl>()) {}

Bencode::~Bencode() {}

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

} // namespace Bencode_Lib
