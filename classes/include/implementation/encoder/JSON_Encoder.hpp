#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "JSON_Translator.hpp"

namespace Bencode_Lib {

class JSON_Encoder final : public IEncoder {
public:
  // Constructors/destructors
  JSON_Encoder() = default;
  JSON_Encoder(const JSON_Encoder &other) = delete;
  JSON_Encoder &operator=(const JSON_Encoder &other) = delete;
  JSON_Encoder(JSON_Encoder &&other) = delete;
  JSON_Encoder &operator=(JSON_Encoder &&other) = delete;
  ~JSON_Encoder() override = default;
  
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
    destination.add('{');
    int commas = BRef<Dictionary>(bNode).value().size();
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      destination.add("\"" + bNodeNext.getKey() + "\"" + " : ");
      encode(bNodeNext.getBNode(), destination);
      if (--commas > 0)
        destination.add(",");
    }
    destination.add('}');
  }

  void encodeList(const BNode &bNode, IDestination &destination) const {
    int commas = BRef<List>(bNode).value().size();
    destination.add('[');
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      encode(bNodeNext, destination);
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
    destination.add(jsonTranslator.to(BRef<String>(bNode).value()));
    destination.add("\"");
  }

  JSON_Translator jsonTranslator;
};
} // namespace Bencode_Lib
