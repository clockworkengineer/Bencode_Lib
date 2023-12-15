#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

class XML_Encoder : public Bencode_Lib::IEncoder {

public:
  // Constructors/destructors
  XML_Encoder() = default;
  XML_Encoder(const XML_Encoder &other) = delete;
  XML_Encoder &operator=(const XML_Encoder &other) = delete;
  XML_Encoder(XML_Encoder &&other) = delete;
  XML_Encoder &operator=(XML_Encoder &&other) = delete;
  ~XML_Encoder() = default;

  void encode(const Bencode_Lib::BNode &bNode,
              Bencode_Lib::IDestination &destination) const {
    destination.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    destination.add("<root>");
    encodeXML(bNode, destination);
    destination.add("</root>");
  }

private:

  std::string removeSpaces(const std::string &elementName) const {
    std::string altered;
    for (auto ch : elementName) {
      if (std::isspace(ch))
        altered += '-';
      else
        altered += ch;
    }
    return (altered);
  }

  void encodeXML(const Bencode_Lib::BNode &bNode,
                 Bencode_Lib::IDestination &destination) const {
    if (bNode.isDictionary()) {
      for (const auto &bNodeNext :
           BRef<Bencode_Lib::Dictionary>(bNode).value()) {
        auto elementName = removeSpaces(bNodeNext.first);
        destination.add("<" + elementName + ">");
        encodeXML(bNodeNext.second, destination);
        destination.add("</" + elementName + ">");
      }
    } else if (bNode.isList()) {
      long index = 0;
      if (BRef<Bencode_Lib::List>(bNode).value().size() > 1) {
        for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).value()) {
          destination.add("<Array" + std::to_string(index) + ">");
          encodeXML(bNodeNext, destination);
          destination.add("</Array" + std::to_string(index) + ">");
          index++;
        }
      } else {
        encodeXML(BRef<Bencode_Lib::List>(bNode).value()[0], destination);
      }
    } else if (bNode.isInteger()) {
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).value()));
    } else if (bNode.isString()) {
      std::string xmlString;
      destination.add("\"");
      if (isStringPrintable(BRef<Bencode_Lib::String>(bNode).value())) {
        xmlString = BRef<Bencode_Lib::String>(bNode).value();
      } else {
        for (unsigned char ch : BRef<Bencode_Lib::String>(bNode).value()) {
          // jsonString += std::format("\\u{:04x}", ch);
          char unicode[7];
          sprintf(unicode, "\\u%04x", ch);
          xmlString += unicode[0];
          xmlString += unicode[1];
          xmlString += unicode[2];
          xmlString += unicode[3];
          xmlString += unicode[4];
          xmlString += unicode[5];
        }
      }
      destination.add(xmlString + "\"");
    }
  }
};
