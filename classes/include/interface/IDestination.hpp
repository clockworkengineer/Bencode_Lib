// File: IDestination.hpp
//
// Description: Interface that defines how Bencode output is delivered to a destination sink.
//
// Implement this interface to customize how Bencode data is written to a target
// buffer, file, or external stream.
#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace Bencode_Lib {

// ====================================================================
// Interface for writing destination stream during Bencode stringification
// ====================================================================
class IDestination {
public:
  // ==================
  // IDestination Error
  // ==================
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view &message)
        : std::runtime_error(
              std::string("IDestination Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IDestination() = default;
  // ========================
  // Add bytes to destination
  // ========================
  virtual void add(const std::string &bytes) { add(std::string_view(bytes)); }
  virtual void add(const std::string_view &bytes) = 0;
  virtual void add(const char *bytes) { add(std::string_view(bytes)); }
  // ============================
  // Add character to destination
  // ============================
  virtual void add(char ch) = 0;
  // ==============================
  // Clear the current destination
  // ==============================
  virtual void clear() = 0;
  // =================================
  // Return the last character written (optional default)
  // =================================
  virtual char last() { return '\0'; }
};
} // namespace Bencode_Lib
