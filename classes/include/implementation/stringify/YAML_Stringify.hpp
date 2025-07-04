#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class YAML_Stringify final : public IStringify {
public:
  // Constructors/destructors
  explicit YAML_Stringify(std::unique_ptr<ITranslator> translator =
                            std::make_unique<Default_Translator>())
      {yamlTranslator = std::move(translator);}
  YAML_Stringify(const YAML_Stringify &other) = delete;
  YAML_Stringify &operator=(const YAML_Stringify &other) = delete;
  YAML_Stringify(YAML_Stringify &&other) = delete;
  YAML_Stringify &operator=(YAML_Stringify &&other) = delete;
  ~YAML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into YAML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified YAML.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
    destination.add("---\n");
    stringifyNodes(bNode, destination, 0);
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
  static void stringifyNodes(const Node &bNode, IDestination &destination,
                  const unsigned long indent) {
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
      throw Error("Unknown Node type encountered during encoding.");
    }
  }
  static void stringifyDictionary(const Node &bNode, IDestination &destination,
                        const unsigned long indent)  {
    if (!BRef<Dictionary>(bNode).value().empty()) {
      for (const auto &entryNode : BRef<Dictionary>(bNode).value()) {
        destination.add(calculateIndent(destination, indent));
        destination.add("\"");
        destination.add(BRef<String>(entryNode.getKeyNode()).value());
        destination.add("\"");
        destination.add(": ");
        if (isA<List>(entryNode.getNode()) ||
            isA<Dictionary>(entryNode.getNode())) {
          destination.add('\n');
        }
        stringifyNodes(entryNode.getNode(), destination, indent + 2);
      }
    } else {
      destination.add("{}\n");
    }
  }
  static void stringifyList(const Node &bNode, IDestination &destination,
                  const unsigned long indent) {
    if (!BRef<List>(bNode).value().empty()) {
      for (const auto &bNodeNext : BRef<List>(bNode).value()) {
        destination.add(calculateIndent(destination, indent) + "- ");
        stringifyNodes(bNodeNext, destination, indent + 2);
      }
    } else {
      destination.add("[]\n");
    }
  }
  static void stringifyInteger(const Node &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value()) + "\n");
  }
  static void stringifyString(const Node &bNode, IDestination &destination)  {
    destination.add("\"" + yamlTranslator->to(BRef<String>(bNode).value()) +
                    "\"" + "\n");
  }

  inline static std::unique_ptr<ITranslator> yamlTranslator;
};
} // namespace Bencode_Lib
