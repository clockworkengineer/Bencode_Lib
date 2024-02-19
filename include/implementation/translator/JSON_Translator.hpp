#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class JSON_Translator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Translator Error: " + message) {}
  };
  // ===============================================
  // Translate string to escape sequences ("\uxxxx")
  // ===============================================
  std::string translate(const std::string toTranslate) const {
    std::string translated;
    for (unsigned char ch : toTranslate) {
      if (!isprint(ch)) {
        const char *digits = "0123456789ABCDEF";
        translated += "\\u00";
        translated += digits[(ch >> 4) & 0x0f];
        translated += digits[(ch)&0x0f];
      } else {
        translated += ch;
      }
    }
    return (translated);
  }
};
} // namespace Bencode_Lib