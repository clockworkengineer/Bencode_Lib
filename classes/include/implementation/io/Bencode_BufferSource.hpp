// File: Bencode_BufferSource.hpp
//
// Description: Source adapter that exposes a string or memory buffer as Bencode input data.
//

#pragma once

#include "interface/ISource.hpp"

#include <cstddef>
#include <string>
#include <string_view>

namespace Bencode_Lib {

class BufferSource final : public ISource {

public:
  // Constructors/Destructors
  explicit BufferSource(const std::string_view &sourceBuffer)
      : parseBuffer(sourceBuffer), parseData(parseBuffer.data()),
        parseSize(parseBuffer.size()) {
    if (parseBuffer.empty()) {
      throw Error("Empty source buffer passed to be parsed.");
    }
  }
  BufferSource() = delete;
  BufferSource(const BufferSource &other) = delete;
  BufferSource &operator=(const BufferSource &other) = delete;
  BufferSource(BufferSource &&other) = delete;
  BufferSource &operator=(BufferSource &&other) = delete;
  ~BufferSource() override = default;

  [[nodiscard]] char current() const override {
    if (more()) {
      return static_cast<char>(parseData[bufferPosition]);
    }
    return EOF;
  }
  void next() override {
    if (!more()) {
      throw Error("Parse buffer empty before parse complete.");
    }
    bufferPosition++;
  }
  [[nodiscard]] bool more() const override {
    return bufferPosition < parseSize;
  }
  void reset() override { bufferPosition = 0; }

private:
  std::size_t bufferPosition = 0;
  std::string parseBuffer;
  const char *parseData = nullptr;
  std::size_t parseSize = 0;
};

} // namespace Bencode_Lib