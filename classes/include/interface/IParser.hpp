#pragma once

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
class ISource;
struct Node;

// =============================
// Interface for Bencode parser
// =============================
class IParser {
public:
  // ==============
  // IParser Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view &message)
        : std::runtime_error(std::string("IParser Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IParser() = default;
  // ==========================================
  // Parse Bencode into Node tree from source
  // ==========================================
  virtual Node parse(ISource &source) = 0;
};
} // namespace Bencode_Lib