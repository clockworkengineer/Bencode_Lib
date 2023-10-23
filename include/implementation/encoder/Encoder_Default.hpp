#pragma once

#include <string>
#include <vector>

#include "Bencode_BNode.hpp"
#include "IDestination.hpp"
#include "IEncoder.hpp"

namespace Bencode_Lib {

class Encoder_Default : public IEncoder {
public:

  Encoder_Default() = default;
  Encoder_Default(const Encoder_Default &other) = delete;
  Encoder_Default &operator=(const Encoder_Default &other) = delete;
  Encoder_Default(Encoder_Default &&other) = delete;
  Encoder_Default &operator=(Encoder_Default &&other) = delete;
  virtual ~Encoder_Default() = default;

  void encode(const BNode &bNode, IDestination &destination)  const {
    if (bNode.is_dictionary()) {
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
};
} // namespace Bencode_Lib