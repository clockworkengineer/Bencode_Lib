// File: Bencode_BufferDestination.hpp
//
// Description: Destination adapter that collects Bencoded output into an in-memory buffer string.
//

#pragma once

#include "interface/IDestination.hpp"

#include <string>
#include <string_view>

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
    encodeBuffer.append(sourceBuffer);
  }
  void add(const std::string_view &sourceBuffer) override {
    encodeBuffer.append(sourceBuffer);
  }
  void add(const char *sourceBuffer) override {
    encodeBuffer.append(sourceBuffer);
  }
  void add(const char ch) override {
    encodeBuffer.push_back(ch);
  }
  void clear() override { encodeBuffer.clear(); }

  [[nodiscard]] std::size_t size() const { return encodeBuffer.size(); }
  std::string toString() const { return encodeBuffer; }
  [[nodiscard]] char last() override { return encodeBuffer.back(); }

private:
  std::string encodeBuffer;
};

} // namespace Bencode_Lib
