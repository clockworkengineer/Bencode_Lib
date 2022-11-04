#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ===========================================================
// Interface for reading source stream during Bencode decoding
// ===========================================================
class ISource {
public:
  // =============
  // ISource Error
  // =============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("ISource Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  ISource() = default;
  ISource(const ISource &other) = delete;
  ISource &operator=(const ISource &other) = delete;
  ISource(ISource &&other) = delete;
  ISource &operator=(ISource &&other) = delete;
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
  virtual void reset() = 0;
};
} // namespace Bencode_Lib