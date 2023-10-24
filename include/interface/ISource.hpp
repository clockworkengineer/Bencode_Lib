#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class ISource {
public:
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