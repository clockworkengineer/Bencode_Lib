//
// Class: Bencode_Impl
//
// Description: Bencode class implementation layer that uses recursion to
// produce a Bencoding tree (decode) and also reconstitute the tree back into
// raw Bencoding bytes (encode).
//
// Dependencies:   C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

/// <summary>
/// Recursively traverse a BNode structure and produce an Bencode encoding of it
/// on the output stream referenced through the IDestination interface.
/// </summary>
/// <param name="bNode">Reference to root of current BNode structure.</param>
/// <param name="destination ">Reference to interface used to facilitate the
/// output stream.</param> <returns></returns>
void Bencode_Impl::encodeBNode(const BNode &bNode, IDestination &destination) {
  if (bNode.is_dictionary()) {
    destination.add('d');
    for (const auto &bNodeEntry : BRef<Dictionary>(bNode).dictionary()) {
      destination.add(std::to_string(bNodeEntry.first.length()) + ":" +
                      bNodeEntry.first);
      encodeBNode(bNodeEntry.second, destination);
    }
    destination.add('e');
  } else if (bNode.is_list()) {
    destination.add('l');
    for (const auto &bNodeEntry : BRef<List>(bNode).list()) {
      encodeBNode(bNodeEntry, destination);
    }
    destination.add('e');
  } else if (bNode.is_integer()) {
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).integer()));
    destination.add('e');
  } else if (bNode.is_string()) {
    destination.add(std::to_string(static_cast<int>(
                        BRef<String>(bNode).string().length())) +
                    ":" + BRef<String>(bNode).string());
  }
}
} // namespace Bencode_Lib
