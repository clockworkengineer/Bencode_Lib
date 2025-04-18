#pragma once

namespace Bencode_Lib {

class BufferSource final : public ISource {

public:
  // Constructors/Destructors
  explicit BufferSource(const std::string &sourceBuffer) {
    if (sourceBuffer.empty()) {
      throw Error("Empty source buffer passed to be parsed.");
    }
    for (auto ch : sourceBuffer) {
      parseBuffer.push_back(static_cast<std::byte>(ch));
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
      return static_cast<char>(parseBuffer[static_cast<int>(bufferPosition)]);
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
    return bufferPosition < parseBuffer.size();
  }
  void reset() override { bufferPosition = 0; }

private:
  std::size_t bufferPosition = 0;
  std::vector<std::byte> parseBuffer;
};

} // namespace Bencode_Lib