// File: Bencode_Impl.cpp
//
// Description: Bencode class implementation layer that manages parser,
// stringifier, and root node state.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode_Impl.hpp"
#include "implementation/stringify/Default_Stringify.hpp"
#include "implementation/parser/Default_Parser.hpp"

namespace Bencode_Lib {

// Need size information for destructor to clean up unique_ptr to
// stringify/parser.
/// <summary>
/// Construct the Bencode implementation with optional stringify and parser objects.
/// </summary>
Bencode_Impl::Bencode_Impl(std::unique_ptr<IStringify> stringify,
                           std::unique_ptr<IParser> parser) {
  if (!stringify) {
    stringify = std::make_unique<Default_Stringify>();
  }
  if (!parser) {
    parser = std::make_unique<Default_Parser>();
  }
  bNodeStringify = std::move(stringify);
  bNodeParser = std::move(parser);
}

/// <summary>
/// Destroy the implementation instance and release owned resources.
/// </summary>
Bencode_Impl::~Bencode_Impl() = default;

namespace {

/// <summary>
/// Build the library version string used by Bencode_Impl::version().
/// </summary>
std::string makeVersionString() {
  return std::string("Bencode_Lib Version ") + std::to_string(BENCODE_VERSION_MAJOR) + "." +
         std::to_string(BENCODE_VERSION_MINOR) + "." + std::to_string(BENCODE_VERSION_PATCH);
}

} // namespace

/// <summary>
/// Return the current library version string.
/// </summary>
std::string Bencode_Impl::version() {
  return makeVersionString();
}

/// <summary>
/// Parse Bencode input from a source reference.
/// </summary>
Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &source) {
  return parseSource(source);
}

/// <summary>
/// Parse Bencode input from an rvalue source.
/// </summary>
Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &&source) {
  return parseSource(std::move(source));
}

/// <summary>
/// Parse implementation that handles exceptions and status conversion.
/// </summary>
Bencode_Impl::ParseResultType Bencode_Impl::parseImpl(ISource &source) {
#if BENCODE_ENABLE_EXCEPTIONS
  bNodeRoot = bNodeParser->parse(source);
  return handleParseResult(source);
#else
  ParseStatus result = bNodeParser->parse(source, bNodeRoot);
  return handleParseResult(std::move(result), source);
#endif
}

#if BENCODE_ENABLE_EXCEPTIONS
/// <summary>
/// Handle parse completion for exception-enabled builds.
/// </summary>
Bencode_Impl::ParseResultType Bencode_Impl::handleParseResult(
    ISource &source) {
  if (source.more()) {
    throw SyntaxError("Source stream terminated early.");
  }
}
#else
/// <summary>
/// Handle parse completion for non-exception builds.
/// </summary>
Bencode_Impl::ParseResultType Bencode_Impl::handleParseResult(
    ParseStatus result, ISource &source) {
  if (!result.ok()) {
    return result;
  }
  if (source.more()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly,
                                "Source stream terminated early.");
  }
  return ParseStatus::success();
}
#endif

/// <summary>
/// Ensure the root is a dictionary, creating it when necessary.
/// </summary>
Node &Bencode_Impl::ensureDictionaryRoot() {
  return ensureRoot<Dictionary>();
}

/// <summary>
/// Ensure the root is a list, creating it when necessary.
/// </summary>
Node &Bencode_Impl::ensureListRoot() {
  return ensureRoot<List>();
}

/// <summary>
/// Stringify using a destination reference.
/// </summary>
void Bencode_Impl::stringify(IDestination &destination) const {
  stringifyImpl(destination);
}

/// <summary>
/// Stringify using an rvalue destination.
/// </summary>
void Bencode_Impl::stringify(IDestination &&destination) const {
  stringifyImpl(destination);
}

/// <summary>
/// Perform stringification into the destination stream.
/// </summary>
void Bencode_Impl::stringifyImpl(IDestination &destination) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencoded data to stringify.");
  }
  bNodeStringify->stringify(bNodeRoot, destination);
}

/// <summary>
/// Ensure there is Bencode data available before traversal or stringify.
/// </summary>
void Bencode_Impl::ensureNotEmpty() const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencode to traverse.");
  }
}

/// <summary>
/// Traverse the Bencode node tree with the provided action.
/// </summary>
void Bencode_Impl::traverseImpl(IAction &action) const {
  ensureNotEmpty();
  traverseNodes(bNodeRoot, action);
}

/// <summary>
/// Traverse using a non-const action implementation.
/// </summary>
void Bencode_Impl::traverse(IAction &action) {
  traverseImpl(action);
}

/// <summary>
/// Traverse using a const action implementation.
/// </summary>
void Bencode_Impl::traverse(IAction &action) const {
  traverseImpl(action);
}

/// <summary>
/// Access or create a dictionary entry by key.
/// </summary>
Node &Bencode_Impl::operator[](const std::string_view &key) {
  return getOrCreateDictionaryEntry(key);
}

/// <summary>
/// Access a const dictionary entry by key without modification.
/// </summary>
const Node &Bencode_Impl::operator[](const std::string_view &key) const {
  return bNodeRoot[key];
}

/// <summary>
/// Access or create a list entry by index.
/// </summary>
Node &Bencode_Impl::operator[](const std::size_t index) {
  return getOrCreateListEntry(index);
}

/// <summary>
/// Access a const list entry by index without modification.
/// </summary>
const Node &Bencode_Impl::operator[](const std::size_t index) const {
  return bNodeRoot[index];
}

/// <summary>
/// Return or create a dictionary entry for the given key.
/// </summary>
Node &Bencode_Impl::getOrCreateDictionaryEntry(const std::string_view &key) {
  return getOrCreateRootEntry<Dictionary>(key);
}

/// <summary>
/// Return or create a list entry for the given index.
/// </summary>
Node &Bencode_Impl::getOrCreateListEntry(std::size_t index) {
  return getOrCreateRootEntry<List>(index);
}

/// <summary>
/// Ensure the internal root container is initialized to the requested type.
/// </summary>
template <typename Container>
Node &Bencode_Impl::ensureRoot() {
  if (bNodeRoot.isEmpty()) {
    bNodeRoot = Node::make<Container>();
  }
  return bNodeRoot;
}

/// <summary>
/// Get or create a root entry for dictionary or list containers.
/// </summary>
template <typename Container, typename Key>
Node &Bencode_Impl::getOrCreateRootEntry(Key &&key) {
  try {
    return ensureRoot<Container>()[std::forward<Key>(key)];
  } catch ([[maybe_unused]] Node::Error &error) {
    if constexpr (std::is_same_v<Container, Dictionary>) {
      NRef<Dictionary>(bNodeRoot).add(
          Dictionary::Entry(std::forward<Key>(key), Node::make<Hole>()));
      return bNodeRoot[std::forward<Key>(key)];
    } else {
      NRef<List>(bNodeRoot).resize(std::forward<Key>(key));
      return bNodeRoot[std::forward<Key>(key)];
    }
  }
}

} // namespace Bencode_Lib
