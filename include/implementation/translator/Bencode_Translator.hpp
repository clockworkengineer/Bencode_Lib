#pragma once

#include <string>
#include <stdexcept>

#include "ITranslator.hpp"

namespace Bencode_Lib {

class Bencode_Translator final : public ITranslator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Bencode Translator Error: " + message) {}
  };
  // ====================
  // Bencode does nothing
  // ====================
  std::string from(const std::string &escapedString) const override {
    return escapedString;
  }
  std::string to(const std::string &rawString)  const override { return rawString; };
};
} // namespace Bencode_Lib