// File: JSON_Stringify.hpp
//
// Description: Header declaring conversion of Bencode node trees into JSON-formatted output.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "implementation/translator/Default_Translator.hpp"

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
  /// Recursively traverse Node structure encoding it into JSON string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified JSON.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
    stringifyNodes(bNode, destination);
  }

private:
  void stringifyNodes(const Node &bNode, IDestination &destination) const {
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
  void stringifyDictionary(const Node &bNode, IDestination &destination) const {
    destination.add('{');
    auto commas = NRef<Dictionary>(bNode).value().size();
    for (const auto &bNodeNext : NRef<Dictionary>(bNode).value()) {
      destination.add("\"");
      destination.add(bNodeNext.getKey());
      destination.add("\" : ");
      stringifyNodes(bNodeNext.getNode(), destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add('}');
  }
  void stringifyList(const Node &bNode, IDestination &destination) const {
    auto commas = NRef<List>(bNode).value().size();
    destination.add('[');
    for (const auto &bNodeNext : NRef<List>(bNode).value()) {
      stringifyNodes(bNodeNext, destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add(']');
  }
  void stringifyInteger(const Node &bNode, IDestination &destination) const {
    destination.add(std::to_string(NRef<Integer>(bNode).value()));
  }
  void stringifyString(const Node &bNode, IDestination &destination) const {
    destination.add("\"");
    destination.add(jsonTranslator ? jsonTranslator->to(NRef<String>(bNode).value()) : NRef<String>(bNode).value());
    destination.add("\"");
  }

  std::unique_ptr<ITranslator> jsonTranslator;
};
} // namespace Bencode_Lib
