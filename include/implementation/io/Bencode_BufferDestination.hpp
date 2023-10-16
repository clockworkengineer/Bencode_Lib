#pragma once

#include <string>
#include <vector>

#include "IDestination.hpp"

namespace Bencode_Lib {

class BufferDestination : public IDestination {
public:
  BufferDestination() = default;
  BufferDestination(const FileSource &other) = delete;
  BufferDestination &operator=(const FileSource &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  virtual ~BufferDestination() = default;

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

private:
  std::vector<std::byte> m_encodeBuffer;
};
} // namespace Bencode_Lib