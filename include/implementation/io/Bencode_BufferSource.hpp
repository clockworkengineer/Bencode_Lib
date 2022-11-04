#pragma once
// =======
// C++ STL
// =======
#include <fstream>
#include <string>
#include <vector>
// ================
// Source interface
// ================
#include "ISource.hpp"
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ================
// CLASS DEFINITION
// ================
class BufferSource : public ISource {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  explicit BufferSource(const std::string &sourceBuffer) {
    if (sourceBuffer.empty()) {
      throw Error("Empty source buffer passed to be decoded.");
    }
    for (auto ch : sourceBuffer) {
      m_decodeBuffer.push_back(static_cast<std::byte>(ch));
    }
  }
  BufferSource() = delete;
  BufferSource(const BufferSource &other) = delete;
  BufferSource &operator=(const BufferSource &other) = delete;
  BufferSource(BufferSource &&other) = delete;
  BufferSource &operator=(BufferSource &&other) = delete;
  virtual ~BufferSource() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  [[nodiscard]] char current() const override {
    if (more()) {
      return (static_cast<char>(
          m_decodeBuffer[static_cast<int>(m_bufferPosition)]));
    }
    return (static_cast<char>(EOF));
  }
  void next() override {
    if (!more()) {
      throw Error("Decode buffer empty before decode complete.");
    }
    m_bufferPosition++;
  }
  [[nodiscard]] bool more() const override {
    return (m_bufferPosition < m_decodeBuffer.size());
  }
  void reset() override { m_bufferPosition = 0; }
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
  std::size_t m_bufferPosition = 0;
  std::vector<std::byte> m_decodeBuffer;
};
} // namespace Bencode_Lib