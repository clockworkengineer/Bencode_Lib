// File: Bencode_BufferDestination.hpp
//
// Description: Destination adapter that collects Bencoded output into an in-memory buffer string.
//

#pragma once

#include "interface/IDestination.hpp"

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

namespace Bencode_Lib {

class FileSource;

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
    const std::size_t size = sourceBuffer.size();
    encodeBuffer.reserve(encodeBuffer.size() + size);
    const auto *begin = reinterpret_cast<const std::byte *>(sourceBuffer.data());
    const auto *end = begin + size;
    encodeBuffer.insert(encodeBuffer.end(), begin, end);
  }
  void add(const std::string_view &sourceBuffer) override {
    const std::size_t size = sourceBuffer.size();
    encodeBuffer.reserve(encodeBuffer.size() + size);
    const auto *begin = reinterpret_cast<const std::byte *>(sourceBuffer.data());
    const auto *end = begin + size;
    encodeBuffer.insert(encodeBuffer.end(), begin, end);
  }
  void add(const char *sourceBuffer) override {
    const std::size_t size = std::strlen(sourceBuffer);
    encodeBuffer.reserve(encodeBuffer.size() + size);
    const auto *begin = reinterpret_cast<const std::byte *>(sourceBuffer);
    const auto *end = begin + size;
    encodeBuffer.insert(encodeBuffer.end(), begin, end);
  }
  void add(const char ch) override {
    encodeBuffer.push_back(static_cast<std::byte>(ch));
  }
  void clear() override { encodeBuffer.clear(); }

  [[nodiscard]] std::size_t size() const { return encodeBuffer.size(); }
  std::string toString() const {
    std::string destination;
    destination.resize(encodeBuffer.size());
    if (!encodeBuffer.empty()) {
      std::memcpy(destination.data(), encodeBuffer.data(), encodeBuffer.size());
    }
    return destination;
  }
  [[nodiscard]] char last() override {
    return static_cast<char>(encodeBuffer.back());
  }

private:
  std::vector<std::byte> encodeBuffer;
};

} // namespace Bencode_Lib
