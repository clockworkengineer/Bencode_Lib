//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

// Need size information for destructor to clean up unique_ptr to
// stringify/parser.
Bencode_Impl::Bencode_Impl(IStringify *stringify, IParser *parser) {
  if (stringify == nullptr) {
    bNodeStringify = std::make_unique<Default_Stringify>();
  } else {
    bNodeStringify.reset(stringify);
  }
  if (parser == nullptr) {
    bNodeParser = std::make_unique<Default_Parser>();
  } else {
    bNodeParser.reset(parser);
  }
}

Bencode_Impl::~Bencode_Impl() = default;

namespace {

std::string makeVersionString() {
  return std::string("Bencode_Lib Version ") + std::to_string(BENCODE_VERSION_MAJOR) + "." +
         std::to_string(BENCODE_VERSION_MINOR) + "." + std::to_string(BENCODE_VERSION_PATCH);
}

} // namespace

std::string Bencode_Impl::version() {
  return makeVersionString();
}

Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &source) {
  return parseSource(source);
}

Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &&source) {
  return parseSource(std::move(source));
}

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
Bencode_Impl::ParseResultType Bencode_Impl::handleParseResult(
    ISource &source) {
  if (source.more()) {
    throw SyntaxError("Source stream terminated early.");
  }
}
#else
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

Node &Bencode_Impl::ensureDictionaryRoot() {
  return ensureRoot<Dictionary>();
}

Node &Bencode_Impl::ensureListRoot() {
  return ensureRoot<List>();
}

void Bencode_Impl::stringify(IDestination &destination) const {
  stringifyImpl(destination);
}

void Bencode_Impl::stringify(IDestination &&destination) const {
  stringifyImpl(destination);
}

void Bencode_Impl::stringifyImpl(IDestination &destination) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencoded data to stringify.");
  }
  bNodeStringify->stringify(bNodeRoot, destination);
}

void Bencode_Impl::ensureNotEmpty() const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencode to traverse.");
  }
}

void Bencode_Impl::traverseImpl(IAction &action) const {
  ensureNotEmpty();
  traverseNodes(bNodeRoot, action);
}

void Bencode_Impl::traverse(IAction &action) {
  traverseImpl(action);
}

void Bencode_Impl::traverse(IAction &action) const {
  traverseImpl(action);
}

Node &Bencode_Impl::operator[](const std::string_view &key) {
  return getOrCreateDictionaryEntry(key);
}

const Node &Bencode_Impl::operator[](const std::string_view &key) const {
  return bNodeRoot[key];
}

Node &Bencode_Impl::operator[](const std::size_t index) {
  return getOrCreateListEntry(index);
}

const Node &Bencode_Impl::operator[](const std::size_t index) const {
  return bNodeRoot[index];
}

Node &Bencode_Impl::getOrCreateDictionaryEntry(const std::string_view &key) {
  return getOrCreateRootEntry<Dictionary>(key);
}

Node &Bencode_Impl::getOrCreateListEntry(std::size_t index) {
  return getOrCreateRootEntry<List>(static_cast<int>(index));
}

template <typename Container>
Node &Bencode_Impl::ensureRoot() {
  if (bNodeRoot.isEmpty()) {
    bNodeRoot = Node::make<Container>();
  }
  return bNodeRoot;
}

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
