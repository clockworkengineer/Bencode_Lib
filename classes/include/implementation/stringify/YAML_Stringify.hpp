
#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Default_Translator.hpp"

namespace Bencode_Lib {

class YAML_Stringify final : public IStringify {
public:
  // Constructors/destructors
  explicit YAML_Stringify(std::unique_ptr<ITranslator> translator =
                            std::make_unique<Default_Translator>())
      : yamlTranslator(std::move(translator)) {}
  YAML_Stringify(const YAML_Stringify &other) = delete;
  YAML_Stringify &operator=(const YAML_Stringify &other) = delete;
  YAML_Stringify(YAML_Stringify &&other) = delete;
  YAML_Stringify &operator=(YAML_Stringify &&other) = delete;
  ~YAML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse BNode structure encoding it into YAML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">BNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified YAML.</param>
  void stringify(const BNode &bNode, IDestination &destination) const override {
    destination.add("---\n");
    stringifyYAML(bNode, destination, 0);
    destination.add("...\n");
  }

private:
  static auto calculateIndent(IDestination &destination,
                              const unsigned long indent) {
    if (destination.last() == '\n') {
      return std::string(indent, ' ');
    }
    return std::string("");
  }
  void stringifyYAML(const BNode &bNode, IDestination &destination,
                  const unsigned long indent) const {
    if (isA<Dictionary>(bNode)) {
      stringifyDictionary(bNode, destination, indent);
    } else if (isA<List>(bNode)) {
      stringifyList(bNode, destination, indent);
    } else if (isA<Integer>(bNode)) {
      stringifyInteger(bNode, destination);
    } else if (isA<String>(bNode)) {
      stringifyString(bNode, destination);
    } else if (isA<Hole>(bNode)) {
    } else {
      throw Error("Unknown BNode type encountered during encoding.");
    }
  }
  void stringifyDictionary(const BNode &bNode, IDestination &destination,
                        const unsigned long indent) const {
    if (!BRef<Dictionary>(bNode).value().empty()) {
      for (const auto &entryBNode : BRef<Dictionary>(bNode).value()) {
        destination.add(calculateIndent(destination, indent));
        destination.add("\"" + BRef<String>(entryBNode.getKeyBNode()).value() +
                        "\"");
        destination.add(": ");
        if (isA<List>(entryBNode.getBNode()) ||
            isA<Dictionary>(entryBNode.getBNode())) {
          destination.add('\n');
        }
        stringifyYAML(entryBNode.getBNode(), destination, indent + 2);
      }
    } else {
      destination.add("{}\n");
    }
  }
  void stringifyList(const BNode &bNode, IDestination &destination,
                  const unsigned long indent) const {
    if (!BRef<List>(bNode).value().empty()) {
      for (const auto &bNodeNext : BRef<List>(bNode).value()) {
        destination.add(calculateIndent(destination, indent) + "- ");
        stringifyYAML(bNodeNext, destination, indent + 2);
      }
    } else {
      destination.add("[]\n");
    }
  }
  static void stringifyInteger(const BNode &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value()) + "\n");
  }
  void stringifyString(const BNode &bNode, IDestination &destination) const {
    destination.add("\"" + yamlTranslator->to(BRef<String>(bNode).value()) +
                    "\"" + "\n");
  }

  std::unique_ptr<ITranslator> yamlTranslator;
};
} // namespace Bencode_Lib
