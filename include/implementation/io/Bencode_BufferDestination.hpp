#pragma once

#include <string>
#include <vector>

#include "IDestination.hpp"

namespace Bencode_Lib {

class BufferDestination : public IDestination {

public:
  // Constructors/Destructors
  BufferDestination() = default;
  BufferDestination(const FileSource &other) = delete;
  BufferDestination &operator=(const FileSource &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  virtual ~BufferDestination() = default;

  void add(const std::string &sourceBuffer) override {
    for (auto ch : sourceBuffer) {
      encodeBuffer.push_back(static_cast<std::byte>(ch));
    }
  }
  void add(const char ch) override {
    encodeBuffer.push_back(static_cast<std::byte>(ch));
  }
  virtual void clear() override { encodeBuffer.clear(); }
  std::vector<std::byte> &getBuffer() { return (encodeBuffer); }
  const std::vector<std::byte> &getBuffer() const { return (encodeBuffer); }
  std::string toString() {
    std::string destination;
    for (auto ch : encodeBuffer) {
      destination.push_back(static_cast<char>(ch));
    }
    return (destination);
  }

private:
  std::vector<std::byte> encodeBuffer;
};

} // namespace Bencode_Lib