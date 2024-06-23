#pragma once

#include <algorithm>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "XML_Translator.hpp"

#pragma once
#include <algorithm>
#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "XML_Translator.hpp"

namespace Bencode_Lib {
class XML_Encoder final : public IEncoder {
public:
  // Constructors/destructors
  XML_Encoder() = default;
  XML_Encoder(const XML_Encoder &other) = delete;
  XML_Encoder &operator=(const XML_Encoder &other) = delete;
  XML_Encoder(XML_Encoder &&other) = delete;
  XML_Encoder &operator=(XML_Encoder &&other) = delete;
  ~XML_Encoder() override = default;

  void encode(const BNode &bNode, IDestination &destination) const override {
    destination.add(R"(<?xml version="1.0" encoding="UTF-8"?>)");
    destination.add("<root>");
    encodeXML(bNode, destination);
    destination.add("</root>");
  }

private:
  void encodeXML(const BNode &bNode, IDestination &destination) const {
    if (bNode.isDictionary()) {
      encodeDictionary(bNode, destination);
    } else if (bNode.isList()) {
      encodeList(bNode, destination);
    } else if (bNode.isInteger()) {
      encodeInteger(bNode, destination);
    } else if (bNode.isString()) {
      encodeString(bNode, destination);
    }
  }

  void encodeDictionary(const BNode &bNode, IDestination &destination) const {
    for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
      auto elementName = bNodeNext.getKey();
      std::ranges::replace(elementName, ' ', '-');
      destination.add("<" + elementName + ">");
      encodeXML(bNodeNext.getBNode(), destination);
      destination.add("</" + elementName + ">");
    }
  }

  void encodeList(const BNode &bNode, IDestination &destination) const {
    if (BRef<List>(bNode).value().size() > 1) {
      for (const auto &bNodeNext : BRef<List>(bNode).value()) {
        destination.add("<Row>");
        encodeXML(bNodeNext, destination);
        destination.add("</Row>");
      }
    } else {
      destination.add("<Row>");
      destination.add("</Row>");
    }
  }

  void encodeInteger(const BNode &bNode, IDestination &destination) const {
    destination.add(std::to_string(BRef<Integer>(bNode).value()));
  }

  void encodeString(const BNode &bNode, IDestination &destination) const {
    destination.add(xmlTranslator.to(BRef<String>(bNode).value()));
  }

  XML_Translator xmlTranslator;
};
} // namespace Bencode_Lib
