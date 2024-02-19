#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class Bencode_Translator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Translator Error: " + message) {}
  };
  // ====================
  // Bencode does nothing
  // ====================
  std::string translate(const std::string toTranslate) const {
    return (toTranslate);
  }
};
} // namespace Bencode_Lib