#pragma once

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
class ISource;
struct BNode;

// =============================
// Interface for Bencode parser
// =============================
class IParser {
public:
  // ==============
  // IParser Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IParser Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IParser() = default;
  // ==========================================
  // Parse Bencode into BNode tree from source
  // ==========================================
  virtual BNode parse(ISource &source) = 0;
};
} // namespace Bencode_Lib