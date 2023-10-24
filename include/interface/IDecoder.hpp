#pragma once

#include <string>
#include <stdexcept>

#include "Bencode_BNode.hpp"

namespace Bencode_Lib {

class IDecoder {
public:
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IDecoder Error: " + message) {}
  };

  IDecoder() = default;
  IDecoder(const IDecoder &other) = delete;
  IDecoder &operator=(const IDecoder &other) = delete;
  IDecoder(IDecoder &&other) = delete;
  IDecoder &operator=(IDecoder &&other) = delete;
  virtual ~IDecoder() = default;

  virtual BNode decode(ISource &source) = 0;

};
} // namespace Bencode_Lib