#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
// =====================
// Destination interface
// =====================
#include "IDestination.hpp"
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ================
// CLASS DEFINITION
// ================
class BufferDestination : public IDestination {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // =====================
  BufferDestination() = default;
  BufferDestination(const FileSource &other) = delete;
  BufferDestination &operator=(const FileSource &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  virtual ~BufferDestination() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  void add(const std::string &sourceBuffer) override {
    for (auto ch : sourceBuffer) {
      m_encodeBuffer.push_back(static_cast<std::byte>(ch));
    }
  }
  void add(const char ch) override {
    m_encodeBuffer.push_back(static_cast<std::byte>(ch));
  }
  std::vector<std::byte> &getBuffer() { return (m_encodeBuffer); }
  const std::vector<std::byte> &getBuffer() const { return (m_encodeBuffer); }
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  // =================
  // PRIVATE VARIABLES
  // =================
  std::vector<std::byte> m_encodeBuffer;
};
} // namespace Bencode_Lib