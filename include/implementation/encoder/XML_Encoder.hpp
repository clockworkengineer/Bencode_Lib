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
  ~XML_Encoder() = default;

  void encode(const BNode &bNode, IDestination &destination) const {
    destination.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    destination.add("<root>");
    encodeXML(bNode, destination);
    destination.add("</root>");
  }

private:
  void encodeXML(const BNode &bNode, IDestination &destination) const {
    if (bNode.isDictionary()) {
      for (const auto &bNodeNext : BRef<Dictionary>(bNode).value()) {
        auto elementName = bNodeNext.getKey();
        std::ranges::replace(elementName, ' ', '-');
        destination.add("<" + elementName + ">");
        encodeXML(bNodeNext.getBNode(), destination);
        destination.add("</" + elementName + ">");
      }
    } else if (bNode.isList()) {
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
    } else if (bNode.isInteger()) {
      destination.add(std::to_string(BRef<Integer>(bNode).value()));
    } else if (bNode.isString()) {
      destination.add(xmlTranslator.to(BRef<String>(bNode).value()));
    }
  }  XML_Translator xmlTranslator;
};

} // namespace Bencode_Lib
