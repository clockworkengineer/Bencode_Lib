#pragma once

#include "Bencode_Core.hpp"

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================

class IDestination;
struct BNode;

// =============================
// Interface for Bencode encoder
// =============================
class IEncoder {
public:
  // ==============
  // IEncoder Error
  // ==============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IEncoder Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IEncoder() = default;
  // =====================================
  // Encode to destination from BNode tree
  // =====================================
  virtual void encode(const BNode &bNode, IDestination &destination) const = 0;
};
} // namespace Bencode_Lib