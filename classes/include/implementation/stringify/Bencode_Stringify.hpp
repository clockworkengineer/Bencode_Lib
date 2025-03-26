#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Default_Translator.hpp"

namespace Bencode_Lib {

class Bencode_Stringify final : public IStringify {
public:
  // Constructors/Destructors
  explicit Bencode_Stringify(std::unique_ptr<ITranslator> translator =
                               std::make_unique<Default_Translator>())
      : bencodeTranslator(std::move(translator)) {}
  Bencode_Stringify(const Bencode_Stringify &other) = delete;
  Bencode_Stringify &operator=(const Bencode_Stringify &other) = delete;
  Bencode_Stringify(Bencode_Stringify &&other) = delete;
  Bencode_Stringify &operator=(Bencode_Stringify &&other) = delete;
  ~Bencode_Stringify() override = default;

  /// <summary>
  /// Recursively traverse BNode structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">BNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified
  /// Bencode.</param>
  void stringify(const BNode &bNode, IDestination &destination) const override {
    if (isA<Dictionary>(bNode)) {
      encodeDictionary(bNode, destination);
    } else if (isA<List>(bNode)) {
      encodeList(bNode, destination);
    } else if (isA<Integer>(bNode)) {
      encodeInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      encodeString(bNode, destination);
    } else if (isA<Hole>(bNode)) {
    } else {
      throw Error("Unknown BNode type encountered during encoding.");
    }
  }

private:
  void encodeDictionary(const BNode &bNode, IDestination &destination) const {
    destination.add('d');
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add(std::to_string(bNodeNext.getKey().length()) + ":" +
                      bNodeNext.getKey());
      stringify(bNodeNext.getBNode(), destination);
    }
    destination.add('e');
  }

  void encodeList(const BNode &bNode, IDestination &destination) const {
    destination.add('l');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      stringify(bNodeNext, destination);
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

  [[maybe_unused]] std::unique_ptr<ITranslator> bencodeTranslator;
};
} // namespace Bencode_Lib
