#pragma once

#include "ITranslator.hpp"

namespace Bencode_Lib {

class XML_Translator final : public ITranslator {
public:
  // ======================================================================
  // Translate string characters to XML character references when necessary
  // ======================================================================
  [[nodiscard]] std::string from(const std::string_view &escapedString) const override {
    return std::string(escapedString);
  }
  [[nodiscard]] std::string to(const std::string_view &escapedString) const override {
    std::string translated;
    for (const  char ch : escapedString) {
      if (!isprint(ch)) {
        translated += "&#x00";
        const auto digits = "0123456789ABCDEF";
        translated += digits[ch >> 4 & 0x0f];
        translated += digits[ch&0x0f];
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
    return translated;
  }
};
} // namespace Bencode_Lib