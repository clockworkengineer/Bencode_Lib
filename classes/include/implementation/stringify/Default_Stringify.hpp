#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Default_Stringify final : public IStringify {
public:
  // Constructors/Destructors
  explicit Default_Stringify(std::unique_ptr<ITranslator> translator =
                               std::make_unique<Default_Translator>())
      : bencodeTranslator(std::move(translator)) {}
  Default_Stringify(const Default_Stringify &other) = delete;
  Default_Stringify &operator=(const Default_Stringify &other) = delete;
  Default_Stringify(Default_Stringify &&other) = delete;
  Default_Stringify &operator=(Default_Stringify &&other) = delete;
  ~Default_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into Bencode string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified
  /// Bencode.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
      stringifyNodes(bNode, destination);
  }

private:
  static void stringifyNodes(const Node &bNode, IDestination &destination)   {
    if (isA<Dictionary>(bNode)) {
      stringifyDictionary(bNode, destination);
    } else if (isA<List>(bNode)) {
      stringifyList(bNode, destination);
    } else if (isA<Integer>(bNode)) {
      stringifyInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      stringifyString(bNode, destination);
    } else if (isA<Hole>(bNode)) {
    } else {
      throw Error("Unknown Node type encountered during encoding.");
    }
  }
  static void stringifyDictionary(const Node &bNode, IDestination &destination)  {
    destination.add('d');
    for (const auto &bNodeNext : NRef<Dictionary>(bNode).value()) {
      destination.add(std::to_string(bNodeNext.getKey().length()) + ":");
      destination.add(bNodeNext.getKey());
      stringifyNodes(bNodeNext.getNode(), destination);
    }
    destination.add('e');
  }
  static void stringifyList(const Node &bNode, IDestination &destination)  {
    destination.add('l');
    for (const auto &bNodeNext : NRef<List>(bNode).value()) {
      stringifyNodes(bNodeNext, destination);
    }
    destination.add('e');
  }
  static void stringifyInteger(const Node &bNode, IDestination &destination) {
    destination.add('i');
    destination.add(std::to_string(NRef<Integer>(bNode).value()));
    destination.add('e');
  }
  static void stringifyString(const Node &bNode, IDestination &destination) {
    destination.add(
        std::to_string(static_cast<int>(NRef<String>(bNode).value().length())) +
        ":");
    destination.add(NRef<String>(bNode).value());
  }

  [[maybe_unused]] std::unique_ptr<ITranslator> bencodeTranslator;
};
} // namespace Bencode_Lib
