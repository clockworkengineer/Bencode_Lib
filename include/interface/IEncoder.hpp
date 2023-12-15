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

protected:
  // =============================================
  // Does string contain all printaable characters
  // =============================================
  bool isStringPrintable(const std::string &str) const {
    for (unsigned char ch : str) {
      if (!isprint(ch)) {
        return (false);
      }
    }
    return (true);
  }
};
} // namespace Bencode_Lib