#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"

#include "IDestination.hpp"
#include "IEncoder.hpp"

class Encoder_JSON : public Bencode_Lib::IEncoder {

public:
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
          jsonString += std ::format("\\u{:04x}", ch);
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
