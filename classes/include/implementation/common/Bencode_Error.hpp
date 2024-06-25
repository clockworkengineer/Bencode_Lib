#pragma once

namespace Bencode_Lib {
// Bencode error types
struct Error final : std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
struct SyntaxError final : std::runtime_error {
  explicit SyntaxError(const std::string &message)
      : std::runtime_error("Bencode Syntax Error: " + message) {}
};

} // namespace Bencode_Lib