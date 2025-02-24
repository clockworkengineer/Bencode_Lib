
#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "YAML_Translator.hpp"

namespace Bencode_Lib {

class YAML_Encoder final : public IEncoder {
public:
  // Constructors/destructors
  YAML_Encoder() = default;
  YAML_Encoder(const YAML_Encoder &other) = delete;
  YAML_Encoder &operator=(const YAML_Encoder &other) = delete;
  YAML_Encoder(YAML_Encoder &&other) = delete;
  YAML_Encoder &operator=(YAML_Encoder &&other) = delete;
  ~YAML_Encoder() override = default;

  void encode(const BNode &bNode, IDestination &destination) const override {
    destination.add("---\n");
    encodeYAML(bNode, destination, 0);
    destination.add("...\n");
  }

private:
  void encodeYAML(const BNode &bNode, IDestination &destination,
                  const unsigned long indent) const {
    if (isA<Dictionary>(bNode)) {
      encodeDictionary(bNode, destination, indent + 2);
    } else if (isA<List>(bNode)) {
      encodeList(bNode, destination, indent + 2);
    } else if (isA<Integer>(bNode)) {
      encodeInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      encodeString(bNode, destination);
    }
  }

  void encodeDictionary(const BNode &bNode, IDestination &destination,
                        const unsigned long indent) const {
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      auto elementName = bNodeNext.getKey();
      destination.add(elementName + ": ");
      encodeYAML(bNodeNext.getBNode(), destination, indent);
    }
  }

  void encodeList(const BNode &bNode, IDestination &destination,
                  const unsigned long indent) const {
    for (const auto &bNodeNext : BRef<List>(bNode).value()) {
      destination.add("- ");
      encodeYAML(bNodeNext, destination, indent);
    }
  }

  static void encodeInteger(const BNode &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value())+"\n");
  }

  void encodeString(const BNode &bNode, IDestination &destination) const {
    destination.add(yamlTranslator.to(BRef<String>(bNode).value())+"\n");
  }

  YAML_Translator yamlTranslator;
};
} // namespace Bencode_Lib
