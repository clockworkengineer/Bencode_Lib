#pragma once

#include <string>
#include <stdexcept>

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
  struct Error final : std::runtime_error {
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
// Make custom encoder to pass to Bencode constructor: Note pointer is tidied up internally.
template <typename T> IEncoder *makeEncoder() {
  return std::make_unique<T>().release();
}
} // namespace Bencode_Lib