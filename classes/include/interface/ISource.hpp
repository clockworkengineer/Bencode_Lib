#pragma once

namespace Bencode_Lib {
  
// =======================================================
// Interface for reading source stream during JSON parsing
// =======================================================
class ISource {
public:
  // =============
  // ISource Error
  // =============
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("ISource Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~ISource() = default;
  // =================
  // Current character
  // =================
  [[nodiscard]] virtual char current() const = 0;
  // ======================
  // Move to next character
  // ======================
  virtual void next() = 0;
  // =========================================
  // Are there still more characters to read ?
  // =========================================
  [[nodiscard]] virtual bool more() const = 0;
  // ===================================
  // Reset to beginning of source stream
  // ===================================
  [[maybe_unused]] virtual void reset() = 0;
};
} // namespace Bencode_Lib