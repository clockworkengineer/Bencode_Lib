#pragma once

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
class IDestination;
struct BNode;

// =============================
// Interface for Bencode stringify
// =============================
class IStringify {
public:
  // ==============
  // IStringify Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view message)
        : std::runtime_error(std::string("IStringify Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IStringify() = default;
  // =====================================
  // Stringify to destination from BNode tree
  // =====================================
  virtual void stringify(const BNode &bNode, IDestination &destination) const = 0;
};
// Make custom stringify
// to pass to Bencode constructor:The note pointer is tidied up internally.
template <typename T> IStringify *makeStringify() {
  return std::make_unique<T>().release();
}
} // namespace Bencode_Lib