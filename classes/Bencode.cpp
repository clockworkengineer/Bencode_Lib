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
// =================
// CLASS DEFINITIONS
// =================
#include "Bencode.hpp"
#include "Bencode_Impl.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
// =========
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
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
/// JSON constructor.
/// </summary>
Bencode::Bencode() : m_implementation(std::make_unique<Bencode_Impl>()) {}
/// <summary>
/// JSON destructor.
/// </summary>
Bencode::~Bencode() {}
/// <summary>
/// Get BencodeLib version
/// </summary>
std::string Bencode::version() const { return (m_implementation->version()); }
/// <summary>
/// Decode Bencoded byte string pointed to by source stream into BNode(s).
/// </summary>
/// <param name="source">Pointer to input interface used to decode Bencoded
/// stream.</param>
void Bencode::decode(ISource &source) { m_implementation->decode(source); }
void Bencode::decode(ISource &&source) { m_implementation->decode(source); }
/// <summary>
/// Take BNode structure and create an Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination ">Pointer to interface used to facilitate the
/// output stream.</param>
void Bencode::encode(IDestination &destination) {

  m_implementation->encode(destination);
}
void Bencode::encode(IDestination &&destination) {

  m_implementation->encode(destination);
}
/// <summary>
/// Return root of Bencoding tree.
/// </summary>
/// <returns>Root of Bencoding tree.</returns>
BNode & Bencode::root() { return (m_implementation->root()); }
[[nodiscard]] const BNode & Bencode::root() const { return (m_implementation->root()); }
} // namespace BencodeLib
