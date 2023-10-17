#pragma once

#include <string>
#include <vector>

#include "ISource.hpp"

namespace Bencode_Lib {

class BufferSource : public ISource {
public:
  explicit BufferSource(const std::string &sourceBuffer) {
    if (sourceBuffer.empty()) {
      throw Error("Empty source buffer passed to be decoded.");
    }
    for (auto ch : sourceBuffer) {
      decodeBuffer.push_back(static_cast<std::byte>(ch));
    }
  }
  BufferSource() = delete;
  BufferSource(const BufferSource &other) = delete;
  BufferSource &operator=(const BufferSource &other) = delete;
  BufferSource(BufferSource &&other) = delete;
  BufferSource &operator=(BufferSource &&other) = delete;
  virtual ~BufferSource() = default;

  [[nodiscard]] char current() const override {
    if (more()) {
      return (
          static_cast<char>(decodeBuffer[static_cast<int>(bufferPosition)]));
    }
    return (static_cast<char>(EOF));
  }
  void next() override {
    if (!more()) {
      throw Error("Decode buffer empty before decode complete.");
    }
    bufferPosition++;
  }
  [[nodiscard]] bool more() const override {
    return (bufferPosition < decodeBuffer.size());
  }
  void reset() override { bufferPosition = 0; }

private:
  std::size_t bufferPosition = 0;
  std::vector<std::byte> decodeBuffer;
};
} // namespace Bencode_Lib