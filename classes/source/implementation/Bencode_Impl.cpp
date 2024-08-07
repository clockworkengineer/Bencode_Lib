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
// encoder/decoder.
Bencode_Impl::Bencode_Impl(IEncoder *encoder, IDecoder *decoder) {
  if (encoder == nullptr) {
    bNodeEncoder = std::make_unique<Bencode_Encoder>();
  } else {
    bNodeEncoder.reset(encoder);
  }
  if (decoder == nullptr) {
    bNodeDecoder = std::make_unique<Bencode_Decoder>();
  } else {
    bNodeDecoder.reset(decoder);
  }
}

Bencode_Impl::~Bencode_Impl() = default;

std::string Bencode_Impl::version() {
  std::stringstream versionString;
  versionString << "Bencode_Lib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return versionString.str();
}

void Bencode_Impl::decode(ISource &source) {
  bNodeRoot = bNodeDecoder->decode(source);
  if (source.more()) {
    throw SyntaxError("Source stream terminated early.");
  }
}

void Bencode_Impl::encode(IDestination &destination) const {
  if (bNodeRoot.isEmpty()) {
    throw Error("No Bencoded data to encode.");
  }
  bNodeEncoder->encode(bNodeRoot, destination);
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
