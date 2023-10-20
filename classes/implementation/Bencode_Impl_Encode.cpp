//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"
#include "Encoder_Default.hpp"

namespace Bencode_Lib {

/// <summary>
/// Recursively traverse a BNode structure and produce an Bencode encoding of it
/// on the output stream referenced through the IDestination interface.
/// </summary>
/// <param name="bNode">Reference to root of current BNode structure.</param>
/// <param name="destination ">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode_Impl::encodeBNode(const BNode &bNode, IDestination &destination) {
  Encoder_Default encoder;
  encoder.encode(bNode, destination);
}
} // namespace Bencode_Lib
