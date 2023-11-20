#pragma once

#include <string>
#include <stdexcept>

#include "ISource.hpp"

#include "Bencode_BNode.hpp"

namespace Bencode_Lib {

class IDecoder {
public:
  // ==============
  // IDecoder Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IDecoder Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  IDecoder() = default;
  IDecoder(const IDecoder &other) = delete;
  IDecoder &operator=(const IDecoder &other) = delete;
  IDecoder(IDecoder &&other) = delete;
  IDecoder &operator=(IDecoder &&other) = delete;
  virtual ~IDecoder() = default;
  // ==========================================
  // Decode Bencode into BNode tree from source
  // ==========================================
  virtual BNode decode(ISource &source) = 0;
};
} // namespace Bencode_Lib