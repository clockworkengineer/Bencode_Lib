//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer that uses recursion to
// produce a Bencoding tree (decode) and also reconstitute the tree back into
// raw Bencoding bytes (encode).
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"
#include "Encoder_Default.hpp"

namespace Bencode_Lib {

/// <summary>
///  Get JSONLib version.
/// </summary>
std::string Bencode_Impl::version() const {
  std::stringstream versionString;
  versionString << "Bencode_Lib Version " << BENCODE_VERSION_MAJOR << "."
                << BENCODE_VERSION_MINOR << "." << BENCODE_VERSION_PATCH;
  return (versionString.str());
}

/// <summary>
/// Decode Bencoded byte string pointed to by source stream into BNode(s).
/// </summary>
/// <param name="source">Reference to input interface used to decode Bencoded
/// stream.</param> <returns></returns>
void Bencode_Impl::decode(ISource &source) { bNodeRoot = decodeBNode(source); }

/// <summary>
/// Take BNode structure and create an Bencode encoding for it in the
/// destination stream.
/// </summary>
/// <param name="destination ">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode_Impl::encode(IDestination &destination) const {
  if (bNodeRoot.getVariant() == nullptr) {
    throw Error("No Bencoded data to encode.");
  }
  // Encoder_Default encoder;
  encoder->encode(bNodeRoot, destination);
}
} // namespace Bencode_Lib
