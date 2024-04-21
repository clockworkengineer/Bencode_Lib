#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {
  // Bencode Lib  error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Bencode Error: " + message) {}
  };

}