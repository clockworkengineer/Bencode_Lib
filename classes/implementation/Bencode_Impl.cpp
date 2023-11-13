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

void Bencode_Impl::setEncoder(IEncoder *encoder) {
  if (encoder == nullptr) {
    bNodeEncoder = std::make_unique<Encoder_Default>();
  } else {
    bNodeEncoder.reset(encoder);
  }
}

void Bencode_Impl::setDecoder(IDecoder *decoder) {
  if (decoder == nullptr) {
    bNodeDecoder = std::make_unique<Decoder_Default>();
  } else {
    bNodeDecoder.reset(decoder);
  }
}

std::string Bencode_Impl::version() const {
  std::stringstream versionString;
  versionString << "Bencode_Lib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return (versionString.str());
}

void Bencode_Impl::decode(ISource &source) {
  bNodeRoot = bNodeDecoder->decode(source);
}

void Bencode_Impl::encode(IDestination &destination) const {
  if (bNodeRoot.getVariant() == nullptr) {
    throw Bencode::Error("No Bencoded data to encode.");
  }
  bNodeEncoder->encode(bNodeRoot, destination);
}

} // namespace Bencode_Lib
