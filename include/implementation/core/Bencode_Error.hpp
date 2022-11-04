#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace Bencode_Lib {
// =============
// Bencode Error
// =============
struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
} // namespace Bencode_Lib