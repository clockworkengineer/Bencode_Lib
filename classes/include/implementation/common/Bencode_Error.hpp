#pragma once

namespace Bencode_Lib {
// Bencode error types
struct Error final : std::runtime_error {
  explicit Error(const std::string_view &message)
      : std::runtime_error(std::string("Bencode Error: ").append(message)) {}
};
struct SyntaxError final : std::runtime_error {
  explicit SyntaxError(const std::string_view &message)
  : std::runtime_error(std::string("Bencode Syntax Error: ").append(message)) {}
};

} // namespace Bencode_Lib