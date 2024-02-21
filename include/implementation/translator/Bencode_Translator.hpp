#pragma once

#include <string>
#include <stdexcept>

#include "ITranslator.hpp"

namespace Bencode_Lib {

class Bencode_Translator : public ITranslator {
public:
  // ==============
  // Translator Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Bencode Translator Error: " + message) {}
  };
  // ====================
  // Bencode does nothing
  // ====================
  std::string from([[maybe_unused]] const std::string &escapedString) override {
    return (escapedString);
  }
  std::string to(const std::string &rawString) override { return (rawString); };
};
} // namespace Bencode_Lib