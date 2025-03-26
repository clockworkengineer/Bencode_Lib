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
    bNodeStringify = std::make_unique<Bencode_Stringify>();
  } else {
    bNodeStringify.reset(stringify);
  }
  if (parser == nullptr) {
    bNodeParser = std::make_unique<Bencode_Parser>();
  } else {
    bNodeParser.reset(parser);
  }
}

Bencode_Impl::~Bencode_Impl() = default;

std::string Bencode_Impl::version() {
  std::stringstream versionString;
  versionString << "Bencode_Lib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return versionString.str();
}

void Bencode_Impl::parse(ISource &source) {
  bNodeRoot = bNodeParser->parse(source);
  if (source.more()) {
    throw SyntaxError("Source stream terminated early.");
  }
}

void Bencode_Impl::stringify(IDestination &destination) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencoded data to stringify.");
  }
  bNodeStringify->stringify(bNodeRoot, destination);
}

void Bencode_Impl::traverse(IAction &action) {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencode to traverse.");
  }
  traverseBNodes(bNodeRoot, action);
}
void Bencode_Impl::traverse(IAction &action) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencode to traverse.");
  }
  traverseBNodes(bNodeRoot, action);
}

BNode &Bencode_Impl::operator[](const std::string &key) {
  try {
    if (bNodeRoot.isEmpty()) {
      bNodeRoot = BNode::make<Dictionary>();
    }
    return bNodeRoot[key];
  } catch ([[maybe_unused]] Dictionary::Error &error) {
    BRef<Dictionary>(bNodeRoot).add(
        Dictionary::Entry(key, BNode::make<Hole>()));
    return bNodeRoot[key];
  }
}
const BNode &Bencode_Impl::operator[](const std::string &key) const {
  return bNodeRoot[key];
}

BNode &Bencode_Impl::operator[](const std::size_t index) {
  try {
    if (bNodeRoot.isEmpty()) {
      bNodeRoot = BNode::make<List>();
    }
    return bNodeRoot[index];
  } catch ([[maybe_unused]] BNode::Error &error) {
    BRef<List>(bNodeRoot).resize(index);
    return bNodeRoot[index];
  }
}
const BNode &Bencode_Impl::operator[](const std::size_t index) const {
  return bNodeRoot[index];
}

} // namespace Bencode_Lib
