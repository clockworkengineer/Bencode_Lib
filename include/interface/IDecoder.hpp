#pragma once

#include <string>
#include <stdexcept>

#include "Bencode_BNode.hpp"

namespace Bencode_Lib {

class IDecoder {
public:
  IDecoder() = default;
  IDecoder(const IDecoder &other) = delete;
  IDecoder &operator=(const IDecoder &other) = delete;
  IDecoder(IDecoder &&other) = delete;
  IDecoder &operator=(IDecoder &&other) = delete;
  virtual ~IDecoder() = default;

  virtual BNode decode(ISource &source) = 0;
};
} // namespace Bencode_Lib