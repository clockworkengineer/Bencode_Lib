#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Translator.hpp"

namespace Bencode_Lib {

class Bencode_Encoder final : public IEncoder {
public:
  // Constructors/Destructors
  Bencode_Encoder() = default;
  Bencode_Encoder(const Bencode_Encoder &other) = delete;
  Bencode_Encoder &operator=(const Bencode_Encoder &other) = delete;
  Bencode_Encoder(Bencode_Encoder &&other) = delete;
  Bencode_Encoder &operator=(Bencode_Encoder &&other) = delete;
  ~Bencode_Encoder() override = default;

  void encode(const BNode &bNode, IDestination &destination) const override {
    if (isA<Dictionary>(bNode)) {
      encodeDictionary(bNode, destination);
    } else if (isA<List>(bNode)) {
      encodeList(bNode, destination);
    } else if (isA<Integer>(bNode)) {
      encodeInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      encodeString(bNode, destination);
    }
  }

private:
  void encodeDictionary(const BNode &bNode, IDestination &destination) const {
    destination.add('d');
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add(
          std::to_string(BRef<String>(bNodeNext.getKey()).value().length()) +
          ":" + BRef<String>(bNodeNext.getKey()).value());
      encode(bNodeNext.getBNode(), destination);
    }
    destination.add('e');
  }
  void encodeList(const BNode &bNode, IDestination &destination) const {
    destination.add('l');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      encode(bNodeNext, destination);
    }
    destination.add('e');
  }
  void encodeInteger(const BNode &bNode, IDestination &destination) const {
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
    destination.add('e');
  }
  void encodeString(const BNode &bNode, IDestination &destination) const {
    destination.add(
        std::to_string(static_cast<int>(BRef<String>(bNode).value().length())) +
        ":" + BRef<String>(bNode).value());
  }

  Bencode_Translator bencodeTranslator;
};
} // namespace Bencode_Lib