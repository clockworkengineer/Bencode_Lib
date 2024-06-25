#pragma once

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
class ISource;
struct BNode;

// =============================
// Interface for Bencode decoder
// =============================
class IDecoder {
public:
  // ==============
  // IDecoder Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IDecoder Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IDecoder() = default;
  // ==========================================
  // Decode Bencode into BNode tree from source
  // ==========================================
  virtual BNode decode(ISource &source) = 0;
};
} // namespace Bencode_Lib