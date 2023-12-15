#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

class JSON_Encoder : public Bencode_Lib::IEncoder {

public:
  // Constructors/destructors
  JSON_Encoder() = default;
  JSON_Encoder(const JSON_Encoder &other) = delete;
  JSON_Encoder &operator=(const JSON_Encoder &other) = delete;
  JSON_Encoder(JSON_Encoder &&other) = delete;
  JSON_Encoder &operator=(JSON_Encoder &&other) = delete;
  ~JSON_Encoder() = default;

  void encode(const Bencode_Lib::BNode &bNode,
              Bencode_Lib::IDestination &destination) const {
    if (bNode.isDictionary()) {
      destination.add('{');
      int commas = BRef<Bencode_Lib::Dictionary>(bNode).value().size();
      for (const auto &bNodeNext :
           BRef<Bencode_Lib::Dictionary>(bNode).value()) {
        destination.add("\"" + bNodeNext.first + "\"" + " : ");
        encode(bNodeNext.second, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add('}');
    } else if (bNode.isList()) {
      int commas = BRef<Bencode_Lib::List>(bNode).value().size();
      destination.add('[');
      for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).value()) {
        encode(bNodeNext, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add(']');
    } else if (bNode.isInteger()) {
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).value()));
    } else if (bNode.isString()) {
      std::string jsonString;
      destination.add("\"");
      if (isStringPrintable(BRef<Bencode_Lib::String>(bNode).value())) {
        jsonString = BRef<Bencode_Lib::String>(bNode).value();
      } else {
        for (unsigned char ch : BRef<Bencode_Lib::String>(bNode).value()) {
          // jsonString += std::format("\\u{:04x}", ch);
          char unicode[7];
          sprintf(unicode, "\\u%04x", ch);
          jsonString += unicode[0];
          jsonString += unicode[1];
          jsonString += unicode[2];
          jsonString += unicode[3];
          jsonString += unicode[4];
          jsonString += unicode[5];
        }
      }
      destination.add(jsonString + "\"");
    }
  }
};
