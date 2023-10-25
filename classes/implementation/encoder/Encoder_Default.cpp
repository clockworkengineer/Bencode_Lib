#include <string>
#include <vector>

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

void Encoder_Default::encode(const BNode &bNode,
                             IDestination &destination) const {
  if (bNode.isDictionary()) {
    destination.add('d');
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).dictionary()) {
      destination.add(std::to_string(bNodeNext.first.length()) + ":" +
                      bNodeNext.first);
      encode(bNodeNext.second, destination);
    }
    destination.add('e');
  } else if (bNode.is_list()) {
    destination.add('l');
    for (const auto &bNodeNext : BRef<List>(bNode).list()) {
      encode(bNodeNext, destination);
    }
    destination.add('e');
  } else if (bNode.isInteger()) {
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).integer()));
    destination.add('e');
  } else if (bNode.isString()) {
    destination.add(std::to_string(static_cast<int>(
                        BRef<String>(bNode).string().length())) +
                    ":" + BRef<String>(bNode).string());
  }
}

} // namespace Bencode_Lib