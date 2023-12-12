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
      if (isPrintableString(BRef<Bencode_Lib::String>(bNode).value())) {
        jsonString = BRef<Bencode_Lib::String>(bNode).value();
      } else {
        for (unsigned char ch : BRef<Bencode_Lib::String>(bNode).value()) {
          // jsonString += std::format("\\u{:04x}", ch);
          char unicode[7];
          sprintf_s(unicode, "\\u%04x", ch);
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

private:
  bool isPrintableString(const std::string &str) const {
    for (unsigned char ch : str) {
      if (!isprint(ch)) {
        return (false);
      }
    }
    return (true);
  }
};
