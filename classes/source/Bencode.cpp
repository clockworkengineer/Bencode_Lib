//
// Class: Bencode
//
// Description: Perform Bencode stringify/parse to/from a byte
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
Bencode::Bencode([[maybe_unused]] IStringify *stringify,
                 [[maybe_unused]] IParser *parser)
    : implementation(std::make_unique<Bencode_Impl>(stringify, parser)) {}
Bencode::~Bencode() = default;
/// <summary>
/// Bencode constructor. Pass a Bencode string to be initially parsed.
/// </summary>
/// <param name="bencodeString">Bencode string.</param>
Bencode::Bencode(const std::string &bencodeString) : Bencode() {
  parse(BufferSource{bencodeString});
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
std::string Bencode::version() { return Bencode_Impl::version(); }
/// <summary>
/// Parse Bencoded byte string pointed to by source stream into BNode(s).
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns></returns>
void Bencode::parse(ISource &source) const { implementation->parse(source); }
void Bencode::parse(ISource &&source) const { implementation->parse(source); }
/// <summary>
/// Take BNode structure and create a Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode::stringify(IDestination &destination) const {
  implementation->stringify(destination);
}
void Bencode::stringify(IDestination &&destination) const {
  implementation->stringify(destination);
}
/// <summary>
/// Recursively traverse JNode structure calling IAction methods (read-only)
///  or to change the Bencode tree node directly.
/// </summary>
/// <param name="action">Action methods to call during traversal.</param>
/// Traverse using non-const Bencode so can change the Bencode tree
[[maybe_unused]] void Bencode::traverse(IAction &action) { implementation->traverse(action); }
// Traverse using const Bencode so cannot change the Bencode tree
void Bencode::traverse(IAction &action) const {
  std::as_const(*implementation).traverse(action);
}
/// <summary>
/// Get the root of BNode tree.
/// </summary>
/// <returns>Root of BNode encoded tree.</returns>
BNode &Bencode::root() { return implementation->root(); }
const BNode &Bencode::root() const { return implementation->root(); }
/// <summary>
/// Return object entry for the passed in keys.
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
/// <summary>
/// Create a Bencode file and write Bencode string to it.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <param name="bencodeString">Bencode string</param>
void Bencode::toFile(const std::string &fileName, const std::string &bencodeString)
{
  Bencode_Impl::toFile(fileName, bencodeString);
}
/// <summary>
/// Open a Bencode file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <returns>Bencode string.</returns>
std::string Bencode::fromFile(const std::string &fileName) { return Bencode_Impl::fromFile(fileName); }
} // namespace Bencode_Lib
