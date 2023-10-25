//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

std::string Bencode_Impl::version() const {
  std::stringstream versionString;
  versionString << "Bencode_Lib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return (versionString.str());
}

void Bencode_Impl::decode(ISource &source) {
  bNodeRoot = decoder->decode(source);
}

void Bencode_Impl::encode(IDestination &destination) const {
  if (bNodeRoot.getVariant() == nullptr) {
    throw Error("No Bencoded data to encode.");
  }
  encoder->encode(bNodeRoot, destination);
}
} // namespace Bencode_Lib
