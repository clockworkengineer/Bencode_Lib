#pragma once

#if defined(BENCODE_ENABLE_FILE_IO)

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

namespace Bencode_Lib {

class FileDestination final : public IDestination {

public:
  // Constructors/Destructors
  explicit FileDestination(const std::string_view filename)
      : filename(filename), destination(nullptr), length(0), lastChar(0) {
#ifdef _MSC_VER
    if (fopen_s(&destination, this->filename.c_str(), "wb") != 0 ||
        !destination) {
#else
    destination = std::fopen(this->filename.c_str(), "wb");
    if (!destination) {
#endif
      throw Error("Bencode file output stream failed to open or could not be "
                  "created.");
    }
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  ~FileDestination() override { close(); }

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
    if (destination) {
      std::fclose(destination);
    }
#ifdef _MSC_VER
    if (fopen_s(&destination, filename.c_str(), "wb") != 0 || !destination) {
#else
    destination = std::fopen(filename.c_str(), "wb");
    if (!destination) {
#endif
      throw Error("File output stream failed to open or could not be created.");
    }
    length = 0;
    lastChar = 0;
  }

  std::size_t size() const { return length; }
  [[nodiscard]] char last() override { return lastChar; }

  std::string getFileName() { return filename; }
  void close() {
    if (destination) {
      std::fclose(destination);
      destination = nullptr;
    }
  }

private:
  void writeBytes(const char *data, std::size_t size) {
    if (!destination) {
      throw Error("File output stream is not open.");
    }
    std::size_t written = std::fwrite(data, 1, size, destination);
    if (written != size) {
      throw Error("Failed to write bytes to file output stream.");
    }
    std::fflush(destination);
    length += written;
    if (size > 0) {
      lastChar = data[size - 1];
    }
  }

  FILE *destination;
  std::string filename;
  std::size_t length{};
  char lastChar{};
};

} // namespace Bencode_Lib

#endif // BENCODE_ENABLE_FILE_IO
