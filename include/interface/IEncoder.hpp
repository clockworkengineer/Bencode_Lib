#pragma once

#include "Bencode_BNode.hpp"
#include "IDestination.hpp"

namespace Bencode_Lib {

class IEncoder {
public:
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IEncoder Error: " + message) {}
  };

  IEncoder() = default;
  IEncoder(const IEncoder &other) = delete;
  IEncoder &operator=(const IEncoder &other) = delete;
  IEncoder(IEncoder &&other) = delete;
  IEncoder &operator=(IEncoder &&other) = delete;
  virtual ~IEncoder() = default;

  // =================================
  // Encode to destination from BNodes
  // =================================
  
  virtual void encode(const BNode &bNode, IDestination &destination) const = 0;
};
} // namespace Bencode_Lib