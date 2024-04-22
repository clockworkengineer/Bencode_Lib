#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {
// Bencode error types
struct Error : public std::runtime_error {
  Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
struct SyntaxError : public std::runtime_error {
  SyntaxError(const std::string &message)
      : std::runtime_error("Bencode Syntax Error: " + message) {}
};

} // namespace Bencode_Lib