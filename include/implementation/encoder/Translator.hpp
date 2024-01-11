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
  std::string
  translateStringToEscapes(const std::string stringToTranslate) const {
    std::string translatedString;
    for (unsigned char ch : stringToTranslate) {
      if (!isprint(ch)) {
        // translatedString += std::format("\\u{:04x}", ch);
        char unicode[7];
        sprintf_s(unicode, "\\u%04x", ch);
        translatedString += unicode[0];
        translatedString += unicode[1];
        translatedString += unicode[2];
        translatedString += unicode[3];
        translatedString += unicode[4];
        translatedString += unicode[5];
      } else {
        translatedString += ch;
      }
    }
    return (translatedString);
  }
};
} // namespace Bencode_Lib