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
  // ===============================================
  // Translate string to escape sequences ("\uxxxx")
  // ===============================================
  std::string
  translateStringToEscapes(const std::string stringToTranslate) const {
    std::string translatedString;
    for (unsigned char ch : stringToTranslate) {
      // translatedString += std::format("\\u{:04x}", ch);
      char unicode[7];
      sprintf_s(unicode, "\\u%04x", ch);
      translatedString += unicode[0];
      translatedString += unicode[1];
      translatedString += unicode[2];
      translatedString += unicode[3];
      translatedString += unicode[4];
      translatedString += unicode[5];
    }
    return (translatedString);
  }
};
} // namespace Bencode_Lib