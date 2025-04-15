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
      : xmlTranslator(std::move(translator)) {}
  XML_Stringify(const XML_Stringify &other) = delete;
  XML_Stringify &operator=(const XML_Stringify &other) = delete;
  XML_Stringify(XML_Stringify &&other) = delete;
  XML_Stringify &operator=(XML_Stringify &&other) = delete;
  ~XML_Stringify() override = default;

  /// <summary>
  /// Recursively traverse BNode structure encoding it into XML string on
  /// the destination stream passed in.
  /// </summary>
  /// <param name="bNode">BNode structure to be traversed.</param>
  /// <param name="destination">Destination stream for stringified XML.</param>
  void stringify(const BNode &bNode, IDestination &destination) const override {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    stringifyXML(bNode, destination);
    destination.add("</root>");
  }

private:
  void stringifyXML(const BNode &bNode, IDestination &destination) const {
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
  void stringifyDictionary(const BNode &bNode, IDestination &destination) const {
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      auto elementName = bNodeNext.getKey();
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      stringifyXML(bNodeNext.getBNode(), destination);
      destination.add("</" + elementName + ">");
    }
  }
  void stringifyList(const BNode &bNode, IDestination &destination) const {
    if (BRef<List>(bNode).value().size() > 1) {
      for (const auto &bNodeNext : BRef<List>(bNode).value()) {
        destination.add("<Row>");
        stringifyXML(bNodeNext, destination);
        destination.add("</Row>");
      }
    } else {
      destination.add("<Row>");
      destination.add("</Row>");
    }
  }
  static void stringifyInteger(const BNode &bNode, IDestination &destination) {
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
  }
  void stringifyString(const BNode &bNode, IDestination &destination) const {
    destination.add(xmlTranslator->to(BRef<String>(bNode).value()));
  }

  std::unique_ptr<ITranslator> xmlTranslator;
};
} // namespace Bencode_Lib
