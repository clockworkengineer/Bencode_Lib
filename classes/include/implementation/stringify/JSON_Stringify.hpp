#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Default_Translator.hpp"

namespace Bencode_Lib {

class JSON_Stringify final : public IStringify {
public:
  // Constructors/destructors
  explicit JSON_Stringify(std::unique_ptr<ITranslator> translator =
                            std::make_unique<Default_Translator>())
      : jsonTranslator(std::move(translator)) {}
  JSON_Stringify(const JSON_Stringify &other) = delete;
  JSON_Stringify &operator=(const JSON_Stringify &other) = delete;
  JSON_Stringify(JSON_Stringify &&other) = delete;
  JSON_Stringify &operator=(JSON_Stringify &&other) = delete;
  ~JSON_Stringify() override = default;

  /// <summary>
  /// Recursively traverse BNode structure encoding it into JSON string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">BNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified JSON.</param>
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
    destination.add('{');
    int commas = BRef<Dictionary>(bNode).value().size();
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add("\"" + bNodeNext.getKey() + "\"" + " : ");
      stringify(bNodeNext.getBNode(), destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add('}');
  }

  void encodeList(const BNode &bNode, IDestination &destination) const {
    int commas = BRef<List>(bNode).value().size();
    destination.add('[');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      stringify(bNodeNext, destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add(']');
  }

  static void encodeInteger(const BNode &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
  }

  void encodeString(const BNode &bNode, IDestination &destination) const {
    destination.add("\"");
    destination.add(jsonTranslator->to(BRef<String>(bNode).value()));
    destination.add("\"");
  }

  std::unique_ptr<ITranslator> jsonTranslator;
};
} // namespace Bencode_Lib
