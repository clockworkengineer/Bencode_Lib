// File: YAML_Stringify.hpp
//
// Description: Header declaring conversion of Bencode node trees into YAML-formatted output.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "implementation/translator/Default_Translator.hpp"

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
  /// Recursively traverse Node structure encoding it into YAML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified YAML.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
    TrackingDestinationAdapter tracker(destination);
    tracker.add("---\n");
    stringifyNodes(bNode, tracker, 0);
    tracker.add("...\n");
  }

private:
  class TrackingDestinationAdapter final : public IDestination {
  public:
    explicit TrackingDestinationAdapter(IDestination &wrapped) : dest(wrapped) {}
    void add(const std::string_view &bytes) override {
      if (!bytes.empty()) {
        lastChar = bytes.back();
      }
      dest.add(bytes);
    }
    void add(char ch) override {
      lastChar = ch;
      dest.add(ch);
    }
    void clear() override {
      lastChar = '\0';
      dest.clear();
    }
    char last() override { return lastChar; }
  private:
    IDestination &dest;
    char lastChar{'\0'};
  };

  static auto calculateIndent(IDestination &destination,
                              const unsigned long indent) {
    if (destination.last() == '\n') {
      return std::string(indent, ' ');
    }
    return std::string("");
  }
  void stringifyNodes(const Node &bNode, IDestination &destination,
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
      throw Error("Unknown Node type encountered during encoding.");
    }
  }
  void stringifyDictionary(const Node &bNode, IDestination &destination,
                                  const unsigned long indent) const {
    if (!NRef<Dictionary>(bNode).value().empty()) {
      for (const auto &entryNode : NRef<Dictionary>(bNode).value()) {
        destination.add(calculateIndent(destination, indent));
        destination.add("\"");
        destination.add(entryNode.getKey());
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
  void stringifyList(const Node &bNode, IDestination &destination,
                            const unsigned long indent) const {
    if (!NRef<List>(bNode).value().empty()) {
      for (const auto &bNodeNext : NRef<List>(bNode).value()) {
        destination.add(calculateIndent(destination, indent) + "- ");
        stringifyNodes(bNodeNext, destination, indent + 2);
      }
    } else {
      destination.add("[]\n");
    }
  }
  void stringifyInteger(const Node &bNode, IDestination &destination) const {
    destination.add(std::to_string(NRef<Integer>(bNode).value()) + "\n");
  }
  void stringifyString(const Node &bNode, IDestination &destination) const {
    destination.add("\"" + (yamlTranslator ? yamlTranslator->to(NRef<String>(bNode).value()) : NRef<String>(bNode).value()) +
                    "\"" + "\n");
  }

  std::unique_ptr<ITranslator> yamlTranslator;
};
} // namespace Bencode_Lib
