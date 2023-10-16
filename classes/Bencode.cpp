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
// Dependencies:   C20++ - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

Bencode::Bencode() : m_implementation(std::make_unique<Bencode_Impl>()) {}

Bencode::~Bencode() {}

std::string Bencode::version() const { return (m_implementation->version()); }

void Bencode::decode(ISource &source) const {
  m_implementation->decode(source);
}
void Bencode::decode(ISource &&source) const {
  m_implementation->decode(source);
}

void Bencode::encode(IDestination &destination) const {
  m_implementation->encode(destination);
}
void Bencode::encode(IDestination &&destination) const {
  m_implementation->encode(destination);
}

BNode &Bencode::root() { return (m_implementation->root()); }
const BNode &Bencode::root() const { return (m_implementation->root()); }

} // namespace Bencode_Lib
