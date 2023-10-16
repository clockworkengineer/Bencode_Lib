#pragma once

#include <stdexcept>

namespace Bencode_Lib {

struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
} // namespace Bencode_Lib