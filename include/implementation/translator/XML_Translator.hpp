#pragma once

#include <string>
#include <stdexcept>

#include "ITranslator.hpp"

namespace Bencode_Lib {

class XML_Translator : public ITranslator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Translator Error: " + message) {}
  };
  // ======================================================================
  // Translate string characters to XML character references when necessary
  // ======================================================================
  std::string from(const std::string &escapedString) const override {
    return (escapedString);
  }
  std::string to(const std::string &escapedString) const override {
    std::string translated;
    for (unsigned char ch : escapedString) {
      if (!isprint(ch)) {
        translated += "&#x00";
        const char *digits = "0123456789ABCDEF";
        translated += digits[(ch >> 4) & 0x0f];
        translated += digits[(ch)&0x0f];
        translated += ";";
      } else {
        if (ch == '&') {
          translated += "&amp;";
        } else if (ch == '<') {
          translated += "&lt;";
        } else if (ch == '>') {
          translated += "&gt;";
        } else if (ch == '\'') {
          translated += "&apos;";
        } else if (ch == '"') {
          translated += "&quot;";
        } else {
          translated += ch;
        }
      }
    }
    return (translated);
  }
};
} // namespace Bencode_Lib