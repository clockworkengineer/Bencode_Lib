#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

class Encoder_XML : public Bencode_Lib::IEncoder {

public:
  // Constructors/destructors
  Encoder_XML() = default;
  Encoder_XML(const Encoder_XML &other) = delete;
  Encoder_XML &operator=(const Encoder_XML &other) = delete;
  Encoder_XML(Encoder_XML &&other) = delete;
  Encoder_XML &operator=(Encoder_XML &&other) = delete;
  ~Encoder_XML() = default;

  void encode(const Bencode_Lib::BNode &bNode,
              Bencode_Lib::IDestination &destination) const {
    destination.add("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
    destination.add("<root>");
    encodeXML(bNode, destination);
    destination.add("</root>");
  }

private:
  bool isPrintableString(const std::string &str) const {
    for (unsigned char ch : str) {
      if (!isprint(ch)) {
        return (false);
      }
    }
    return (true);
  }

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
      int commas = BRef<Bencode_Lib::List>(bNode).value().size();
      destination.add('[');
      for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).value()) {
        encodeXML(bNodeNext, destination);
        if (--commas > 0)
          destination.add(",");
      }
    } else if (bNode.isInteger()) {
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).value()));
    } else if (bNode.isString()) {
      std::string xmlString;
      destination.add("\"");
      if (isPrintableString(BRef<Bencode_Lib::String>(bNode).value())) {
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
