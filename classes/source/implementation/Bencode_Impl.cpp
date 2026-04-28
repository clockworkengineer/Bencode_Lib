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

std::string Bencode_Impl::version() {
  return std::string("Bencode_Lib Version ") + std::to_string(BENCODE_VERSION_MAJOR) + "." +
         std::to_string(BENCODE_VERSION_MINOR) + "." + std::to_string(BENCODE_VERSION_PATCH);
}

Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &source) {
  return parseImpl(source);
}

Bencode_Impl::ParseResultType Bencode_Impl::parse(ISource &&source) {
  return parseImpl(source);
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
  if (bNodeRoot.isEmpty()) {
    bNodeRoot = Node::make<Dictionary>();
  }
  return bNodeRoot;
}

Node &Bencode_Impl::ensureListRoot() {
  if (bNodeRoot.isEmpty()) {
    bNodeRoot = Node::make<List>();
  }
  return bNodeRoot;
}

void Bencode_Impl::stringify(IDestination &destination) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencoded data to stringify.");
  }
  bNodeStringify->stringify(bNodeRoot, destination);
}

void Bencode_Impl::stringify(IDestination &&destination) const {
  stringify(destination);
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
  try {
    return ensureDictionaryRoot()[key];
  } catch ([[maybe_unused]] Node::Error &error) {
    NRef<Dictionary>(bNodeRoot).add(Dictionary::Entry(key, Node::make<Hole>()));
    return bNodeRoot[key];
  }
}

const Node &Bencode_Impl::operator[](const std::string_view &key) const {
  return bNodeRoot[key];
}

Node &Bencode_Impl::operator[](const std::size_t index) {
  try {
    return ensureListRoot()[index];
  } catch ([[maybe_unused]] Node::Error &error) {
    NRef<List>(bNodeRoot).resize(index);
    return bNodeRoot[index];
  }
}

const Node &Bencode_Impl::operator[](const std::size_t index) const {
  return bNodeRoot[index];
}

} // namespace Bencode_Lib
