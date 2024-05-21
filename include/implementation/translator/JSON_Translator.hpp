#pragma once

#include <string>
#include <stdexcept>

#include "ITranslator.hpp"

namespace Bencode_Lib {

class JSON_Translator final : public ITranslator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Translator Error: " + message) {}
  };
  // ===============================================
  // Translate string to escape sequences ("\uxxxx")
  // ===============================================
  std::string from(const std::string &escapedString) const override {
    return escapedString;
  }
  std::string to(const std::string &escapedString) const override {
    std::string translated;
    for (const unsigned char ch : escapedString) {
      if (!isprint(ch)) {
        const char *digits = "0123456789ABCDEF";
        translated += "\\u00";
        translated += digits[ch >> 4 & 0x0f];
        translated += digits[ch&0x0f];
      } else {
        translated += ch;
      }
    }
    return translated;
  }
};
} // namespace Bencode_Lib