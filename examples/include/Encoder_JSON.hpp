#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"

#include "IEncoder.hpp"

class Encoder_JSON : public Bencode_Lib::IEncoder {

public:
  // Constructors/destructors
  Encoder_JSON() = default;
  Encoder_JSON(const Encoder_JSON &other) = delete;
  Encoder_JSON &operator=(const Encoder_JSON &other) = delete;
  Encoder_JSON(Encoder_JSON &&other) = delete;
  Encoder_JSON &operator=(Encoder_JSON &&other) = delete;
  ~Encoder_JSON() = default;

  void encode(const Bencode_Lib::BNode &bNode,
              Bencode_Lib::IDestination &destination) const {
    if (bNode.isDictionary()) {
      destination.add('{');
      int commas = BRef<Bencode_Lib::Dictionary>(bNode).dictionary().size();
      for (const auto &bNodeNext :
           BRef<Bencode_Lib::Dictionary>(bNode).dictionary()) {
        destination.add("\"" + bNodeNext.first + "\"" + " : ");
        encode(bNodeNext.second, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add('}');
    } else if (bNode.isList()) {
      int commas = BRef<Bencode_Lib::List>(bNode).list().size();
      destination.add('[');
      for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).list()) {
        encode(bNodeNext, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add(']');
    } else if (bNode.isInteger()) {
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).integer()));
    } else if (bNode.isString()) {
      std::string jsonString;
      destination.add("\"");
      if (isPrintableString(BRef<Bencode_Lib::String>(bNode).string())) {
        jsonString = BRef<Bencode_Lib::String>(bNode).string();
      } else {
        for (unsigned char ch : BRef<Bencode_Lib::String>(bNode).string()) {
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
