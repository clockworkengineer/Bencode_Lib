#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class Translator {
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
  std::string translateToEscapes(const std::string toTranslate) const {
    std::string translated;
    for (unsigned char ch : toTranslate) {
      if (!isprint(ch)) {
        char escaped[7];
        sprintf_s(escaped, "\\u%04x", ch);
        translated += escaped[0];
        translated += escaped[1];
        translated += escaped[2];
        translated += escaped[3];
        translated += escaped[4];
        translated += escaped[5];
      } else {
        translated += ch;
      }
    }
    return (translated);
  }
};
} // namespace Bencode_Lib