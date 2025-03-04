#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

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

  /// <summary>
  /// Recursively traverse BNode structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">BNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified Bencode.</param>
  /// <param name="indent">Current print indentation.</param>
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
          std::to_string(bNodeNext.getKey().length()) +
          ":" + bNodeNext.getKey());
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

  static void encodeInteger(const BNode &bNode, IDestination &destination) {
    destination.add('i');
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
    destination.add('e');
  }

  static void encodeString(const BNode &bNode, IDestination &destination) {
    destination.add(
        std::to_string(static_cast<int>(BRef<String>(bNode).value().length())) +
        ":" + BRef<String>(bNode).value());
  }

};
} // namespace Bencode_Lib
