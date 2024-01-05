#include <string>
#include <vector>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Encoder.hpp"

namespace Bencode_Lib {

void Bencode_Encoder::encode(const BNode &bNode,
                             IDestination &destination) const {
  if (bNode.isDictionary()) {
    destination.add('d');
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add(std::to_string(bNodeNext.getKey().length()) + ":" +
                      bNodeNext.getKey());
      encode(bNodeNext.getBNode(), destination);
    }
    destination.add('e');
  } else if (bNode.isList()) {
    destination.add('l');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      encode(bNodeNext, destination);
    }
    destination.add('e');
  } else if (bNode.isInteger()) {
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
    destination.add('e');
  } else if (bNode.isString()) {
    destination.add(
        std::to_string(static_cast<int>(BRef<String>(bNode).value().length())) +
        ":" + BRef<String>(bNode).value());
  }
}

} // namespace Bencode_Lib