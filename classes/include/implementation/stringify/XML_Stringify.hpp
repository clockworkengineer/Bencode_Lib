#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "XML_Translator.hpp"

namespace Bencode_Lib {

class XML_Stringify final : public IStringify {
public:
  // Constructors/destructors
  explicit XML_Stringify(std::unique_ptr<ITranslator> translator =
                           std::make_unique<XML_Translator>())
        {xmlTranslator = std::move(translator);}
  XML_Stringify(const XML_Stringify &other) = delete;
  XML_Stringify &operator=(const XML_Stringify &other) = delete;
  XML_Stringify(XML_Stringify &&other) = delete;
  XML_Stringify &operator=(XML_Stringify &&other) = delete;
  ~XML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse Node structure encoding it into XML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">Node structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified XML.</param>
  void stringify(const Node &bNode, IDestination &destination) const override {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    stringifyNodes(bNode, destination);
    destination.add("</root>");
  }

private:
  static void stringifyNodes(const Node &bNode, IDestination &destination)  {
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
    for (const auto &bNodeNext : NRef<Dictionary>(bNode).value()) {
      auto elementName = std::string(bNodeNext.getKey());
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      stringifyNodes(bNodeNext.getNode(), destination);
      destination.add("</" + elementName + ">");
    }
  }
  static void stringifyList(const Node &bNode, IDestination &destination)  {
    if (NRef<List>(bNode).value().size() > 1) {
      for (const auto &bNodeNext : NRef<List>(bNode).value()) {
        destination.add("<Row>");
        stringifyNodes(bNodeNext, destination);
        destination.add("</Row>");
      }
    } else {
      destination.add("<Row>");
      destination.add("</Row>");
    }
  }
  static void stringifyInteger(const Node &bNode, IDestination &destination) {
    destination.add(std::to_string(NRef<Integer>(bNode).value()));
  }
  static void stringifyString(const Node &bNode, IDestination &destination)  {
    destination.add(xmlTranslator->to(NRef<String>(bNode).value()));
  }

  inline static std::unique_ptr<ITranslator> xmlTranslator;
};
} // namespace Bencode_Lib
