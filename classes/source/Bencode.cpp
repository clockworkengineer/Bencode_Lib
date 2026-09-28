// File: Bencode.cpp
//
// Description: Perform Bencode stringify/parse to/from a byte
// buffer or file. Supports public Bencode API operations and file
// I/O helpers.
//
// Class: Bencode
//
// Dependencies: C++20 - Language standard features used.
//
#include "Bencode_Impl.hpp"
#include "implementation/parser/Sax_Parser.hpp"

namespace Bencode_Lib {

/// <summary>
/// Initialise the implementation layer.
Bencode::Bencode()
    : implementation(std::make_unique<Bencode_Impl>()) {}

Bencode::Bencode(std::unique_ptr<IStringify> stringify)
    : implementation(std::make_unique<Bencode_Impl>(std::move(stringify))) {}

Bencode::Bencode(std::unique_ptr<IParser> parser)
    : implementation(std::make_unique<Bencode_Impl>(std::move(parser))) {}

Bencode::Bencode(std::unique_ptr<IStringify> stringify,
                 std::unique_ptr<IParser> parser)
    : implementation(
      std::make_unique<Bencode_Impl>(std::move(stringify), std::move(parser))) {}

Bencode::Bencode(std::nullptr_t)
    : implementation(std::make_unique<Bencode_Impl>()) {}
void Bencode::ensureImplementation() const {
  if (!implementation) {
    implementation = std::make_unique<Bencode_Impl>();
  }
}

Bencode::Bencode(Bencode &&other) noexcept = default;
Bencode &Bencode::operator=(Bencode &&other) noexcept = default;
Bencode::~Bencode() = default;
/// <summary>
/// Bencode constructor. Pass a Bencode string to be initially parsed.
/// </summary>
/// <param name="bencodeString">Bencode string.</param>
Bencode::Bencode(const std::string_view &bencodeString) : Bencode() {
  (void)parse(BufferSource{bencodeString});
}
/// <summary>
/// Bencode constructor (list).
/// </summary>
/// <param name="list">Initializer list of single values or JNode.</param>
Bencode::Bencode(const ListInitializerType &list) : Bencode() {
  this->root() = Node(list);
}
/// <summary>
/// Bencode constructor (dictionary).
/// </summary>
/// <param name="dictionary">Initializer list of key/value(JNode) pairs.</param>
Bencode::Bencode(const DictionaryInitializerType &dictionary) : Bencode() {
  this->root() = Node(dictionary);
}
/// <summary>
///  Get Bencode_Lib version.
/// </summary>
std::string Bencode::version() { return Bencode_Impl::version(); }
/// <summary>
/// Parse Bencoded byte string pointed to by source stream into Node(s).
/// </summary>
/// <param name="source">Reference to input interface used to parse Bencoded
/// stream.</param> <returns></returns>
Bencode::ParseResultType Bencode::parse(ISource &source) const {
  ensureImplementation();
  return implementation->parse(source);
}

/// <summary>
/// Implementation of the rvalue source overload for parse.
/// </summary>
Bencode::ParseResultType Bencode::parse(ISource &&source) const {
  ensureImplementation();
  return implementation->parse(std::move(source));
}

/// <summary>
/// Parse Bencode directly from a string_view buffer.
/// </summary>
Bencode::ParseResultType Bencode::parse(const std::string_view &bencodeString) const {
  return parse(BufferSource{bencodeString});
}

/// <summary>
/// Stream Bencode events directly into an ISaxHandler from an ISource reference.
/// </summary>
Bencode::SaxResultType Bencode::parseSax(ISource &source, ISaxHandler &handler) {
#if BENCODE_ENABLE_EXCEPTIONS
  return SaxParser::parseOrThrow(source, handler, getMaxParserDepth());
#else
  return SaxParser::parse(source, handler, getMaxParserDepth());
#endif
}

/// <summary>
/// Stream Bencode events directly into an ISaxHandler from an rvalue ISource.
/// </summary>
Bencode::SaxResultType Bencode::parseSax(ISource &&source, ISaxHandler &handler) {
  return parseSax(source, handler);
}

/// <summary>
/// Stream Bencode events directly into an ISaxHandler from a string_view buffer.
/// </summary>
Bencode::SaxResultType Bencode::parseSax(const std::string_view &bencodeString, ISaxHandler &handler) {
#if BENCODE_ENABLE_EXCEPTIONS
  return SaxParser::parseOrThrow(bencodeString, handler, getMaxParserDepth());
#else
  return SaxParser::parse(bencodeString, handler, getMaxParserDepth());
#endif
}

/// <summary>
/// Parse raw Bencode payload into destination BencodeView returning ParseStatus.
/// </summary>
ParseStatus Bencode::parseView(std::string_view bencodeString, BencodeView &destination) {
  return BencodeView::parse(bencodeString, destination, getMaxParserDepth());
}

#if BENCODE_ENABLE_EXCEPTIONS
/// <summary>
/// Parse raw Bencode payload into a zero-copy BencodeView tree.
/// </summary>
BencodeView Bencode::parseView(std::string_view bencodeString) {
  return BencodeView::parse(bencodeString, getMaxParserDepth());
}
#endif

/// <summary>
/// Take Node structure and create a Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode::stringify(IDestination &destination) const {
  ensureImplementation();
  implementation->stringify(destination);
}
/// <summary>
/// Implementation of the rvalue destination overload for stringify.
/// </summary>
void Bencode::stringify(IDestination &&destination) const {
  ensureImplementation();
  implementation->stringify(std::move(destination));
}

#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
/// <summary>
/// Convenience stringification returning an std::string.
/// </summary>
std::string Bencode::stringify() const {
  BufferDestination destination;
  stringify(destination);
  return destination.toString();
}

/// <summary>
/// Synonym for stringify().
/// </summary>
std::string Bencode::encode() const {
  return stringify();
}
#endif

/// <summary>
/// Recursively traverse JNode structure calling IAction methods (read-only)
///  or to change the Bencode tree node directly.
/// </summary>
/// <param name="action">Action methods to call during traversal.</param>
/// Traverse using non-const Bencode so can change the Bencode tree
[[maybe_unused]] void Bencode::traverse(IAction &action) {
  ensureImplementation();
  implementation->traverse(action);
}
// Traverse using const Bencode so cannot change the Bencode tree
void Bencode::traverse(IAction &action) const {
  ensureImplementation();
  std::as_const(*implementation).traverse(action);
}
/// <summary>
/// Get the root of Node tree.
/// </summary>
/// <returns>Root of Node encoded tree.</returns>
Node &Bencode::root() {
  ensureImplementation();
  return implementation->root();
}
/// <summary>
/// Retrieve the constant root node of the parsed Bencode tree.
/// </summary>
const Node &Bencode::root() const {
  ensureImplementation();
  return implementation->root();
}
/// <summary>
/// Return object entry for the passed in keys.
/// </summary>
/// <param name="key">Dictionary entry (Node) key.</param>
Node &Bencode::operator[](const std::string_view &key) {
  ensureImplementation();
  return (*implementation)[key];
}
/// <summary>
/// Retrieve a constant dictionary entry by key.
/// </summary>
const Node &Bencode::operator[](const std::string_view &key) const {
  ensureImplementation();
  return (*implementation)[key];
}
/// <summary>
/// Return list entry for the passed in index.
/// </summary>
/// <param name="index">Array entry (Node) index.</param>
Node &Bencode::operator[](const std::size_t index) {
  ensureImplementation();
  return (*implementation)[index];
}
/// <summary>
/// Retrieve a constant list entry by index.
/// </summary>
const Node &Bencode::operator[](const std::size_t index) const {
  ensureImplementation();
  return (*implementation)[index];
}
/// <summary>
/// Create a Bencode file and write Bencode string to it.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <param name="bencodeString">Bencode string</param>
void Bencode::toFile(const std::string_view &fileName,
                     const std::string_view &bencodeString) {
  Bencode_Impl::toFile(fileName, bencodeString);
}
/// <summary>
/// Open a Bencode file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <returns>Bencode string.</returns>
std::string Bencode::fromFile(const std::string_view &fileName) {
  return Bencode_Impl::fromFile(fileName);
}

/// <summary>
/// Set the maximum parser recursion depth.
/// </summary>
void Bencode::setMaxParserDepth(const unsigned long depth) {
  Bencode_Impl::setMaxParserDepth(depth);
}

/// <summary>
/// Get the maximum parser recursion depth.
/// </summary>
unsigned long Bencode::getMaxParserDepth() {
  return Bencode_Impl::getMaxParserDepth();
}

} // namespace Bencode_Lib
