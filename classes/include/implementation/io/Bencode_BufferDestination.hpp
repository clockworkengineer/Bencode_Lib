#pragma once

#include "IDestination.hpp"

namespace Bencode_Lib {

class BufferDestination final : public IDestination {

public:
  // Constructors/Destructors
  BufferDestination() = default;
  explicit BufferDestination(const FileSource &other) = delete;
  BufferDestination &operator=(const FileSource &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  ~BufferDestination() override = default;

  void add(const std::string &sourceBuffer) override {
    for (auto ch : sourceBuffer) {
      encodeBuffer.push_back(static_cast<std::byte>(ch));
    }
  }
  void add(const char ch) override {
    encodeBuffer.push_back(static_cast<std::byte>(ch));
  }
  void clear() override { encodeBuffer.clear(); }

  [[nodiscard]] std::size_t size() const { return encodeBuffer.size(); }
  std::string toString() {
    std::string destination;
    for (auto ch : encodeBuffer) {
      destination.push_back(static_cast<char>(ch));
    }
    return destination;
  }
  [[nodiscard]] char last() override { return static_cast<char>(encodeBuffer.back()); }
  
private:
  std::vector<std::byte> encodeBuffer;
};

} // namespace Bencode_Lib
