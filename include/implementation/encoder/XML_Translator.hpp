#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class XML_Translator {
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
  std::string translate(const std::string toTranslate) const {
    std::string translated;
    for (unsigned char ch : toTranslate) {
      if (!isprint(ch)) {
        char escaped[5];
        translated += "&#x";
        sprintf_s(escaped, "%04x", ch);
        translated += escaped[0];
        translated += escaped[1];
        translated += escaped[2];
        translated += escaped[3];
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