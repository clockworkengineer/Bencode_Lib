#pragma once
//
// C++ STL
//
#include <stdexcept>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// =============
// Bencode Error
// =============
struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
} // namespace Bencode_Lib