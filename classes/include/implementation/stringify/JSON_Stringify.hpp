#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class JSON_Stringify final : public IStringify {
public:
  // Constructors/destructors
  explicit JSON_Stringify(std::unique_ptr<ITranslator> translator =
                            std::make_unique<Default_Translator>())
  {jsonTranslator = std::move(translator);}
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
    stringifyBNodes(bNode, destination);
  }

private:
  static void stringifyBNodes(const BNode &bNode, IDestination &destination)   {
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
      throw Error("Unknown BNode type encountered during encoding.");
    }
  }
  static void stringifyDictionary(const BNode &bNode, IDestination &destination)  {
    destination.add('{');
    int commas = BRef<Dictionary>(bNode).value().size();
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add("\"");
      destination.add(bNodeNext.getKey());
      destination.add("\" : ");
      stringifyBNodes(bNodeNext.getBNode(), destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add('}');
  }
  static void stringifyList(const BNode &bNode, IDestination &destination)  {
    int commas = BRef<List>(bNode).value().size();
    destination.add('[');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      stringifyBNodes(bNodeNext, destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add(']');
  }
  static void stringifyInteger(const BNode &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
  }
  static void stringifyString(const BNode &bNode, IDestination &destination)  {
    destination.add("\"");
    destination.add(jsonTranslator->to(BRef<String>(bNode).value()));
    destination.add("\"");
  }

  inline static std::unique_ptr<ITranslator> jsonTranslator;
};
} // namespace Bencode_Lib
