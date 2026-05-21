// File: Bencode_FileDestination.hpp
//
// Description: Destination adapter for writing stringified Bencode output to a filesystem file.
//

#pragma once

#if BENCODE_ENABLE_FILE_IO

#include "interface/IDestination.hpp"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <string_view>

namespace Bencode_Lib {

class FileDestination final : public IDestination {

public:
  // Constructors/Destructors
  explicit FileDestination(const std::string_view filename)
      : filename(filename), length(0), lastChar(0) {
#ifdef _MSC_VER
    FILE *rawDestination = nullptr;
    if (fopen_s(&rawDestination, this->filename.c_str(), "wb") != 0 ||
        !rawDestination) {
#else
    FILE *rawDestination = std::fopen(this->filename.c_str(), "wb");
    if (!rawDestination) {
#endif
      throw Error("Bencode file output stream failed to open or could not be "
                  "created.");
    }
    destination.reset(rawDestination);
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  ~FileDestination() override = default;

  void add(const std::string &bytes) override {
    writeBytes(bytes.data(), bytes.length());
  }
  void add(const std::string_view &bytes) override {
    writeBytes(bytes.data(), bytes.length());
  }
  void add(const char *bytes) override {
    writeBytes(bytes, std::strlen(bytes));
  }
  void add(const char ch) override { writeBytes(&ch, 1); }

  void clear() override {
    destination.reset();
#ifdef _MSC_VER
    FILE *rawDestination = nullptr;
    if (fopen_s(&rawDestination, filename.c_str(), "wb") != 0 || !rawDestination) {
#else
    FILE *rawDestination = std::fopen(filename.c_str(), "wb");
    if (!rawDestination) {
#endif
      throw Error("File output stream failed to open or could not be created.");
    }
    destination.reset(rawDestination);
    length = 0;
    lastChar = 0;
  }

  std::size_t size() const { return length; }
  [[nodiscard]] char last() override { return lastChar; }

  std::string getFileName() { return filename; }
  void close() {
    destination.reset();
  }

private:
  void writeBytes(const char *data, std::size_t size) {
    if (!destination) {
      throw Error("File output stream is not open.");
    }
    std::size_t written = std::fwrite(data, 1, size, destination.get());
    if (written != size) {
      throw Error("Failed to write bytes to file output stream.");
    }
    std::fflush(destination.get());
    length += written;
    if (size > 0) {
      lastChar = data[size - 1];
    }
  }

  std::unique_ptr<FILE, decltype(&std::fclose)> destination{nullptr, &std::fclose};
  std::string filename;
  std::size_t length{};
  char lastChar{};
};

} // namespace Bencode_Lib

#endif // BENCODE_ENABLE_FILE_IO
