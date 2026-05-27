// File: Bencode_FileSource.hpp
//
// Description: Source adapter for reading Bencode input from a filesystem file path.
//

#pragma once

#if BENCODE_ENABLE_FILE_IO

#include "interface/ISource.hpp"

#include <cstdio>
#include <memory>
#include <string>
#include <string_view>

namespace Bencode_Lib {

class FileSource final : public ISource {

public:
  // Constructors/Destructors
  explicit FileSource(const std::string_view &sourceFileName)
      : currentChar(EOF), hasPeek(false), filename(sourceFileName) {
#ifdef _MSC_VER
    FILE *rawSource = nullptr;
    if (fopen_s(&rawSource, filename.c_str(), "rb") != 0 || !rawSource) {
#else
    FILE *rawSource = std::fopen(filename.c_str(), "rb");
    if (!rawSource) {
#endif
      throw Error(
          "Bencode file input stream failed to open or does not exist.");
    }
    source.reset(rawSource);
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  ~FileSource() override = default;

  char current() const override {
    ensurePeek();
    return currentChar != EOF ? static_cast<char>(currentChar) : EOF;
  }
  void next() override {
    if (!more()) {
      throw Error("Parse buffer empty before parse complete.");
    }
    if (!hasPeek) {
      ensurePeek();
    }
    std::fgetc(source.get());
    hasPeek = false;
  }
  bool more() const override {
    ensurePeek();
    return currentChar != EOF;
  }
  void reset() override {
    if (source) {
      std::rewind(source.get());
      hasPeek = false;
      currentChar = EOF;
    }
  }
  std::string getFileName() { return filename; }
  void close() {
    if (source) {
      source.reset();
      hasPeek = false;
      currentChar = EOF;
    }
  }

private:
  void ensurePeek() const {
    if (hasPeek || !source) {
      return;
    }
    currentChar = std::fgetc(source.get());
    if (currentChar != EOF) {
      std::ungetc(currentChar, source.get());
    }
    hasPeek = true;
  }

  using FileDeleter = int (*)(FILE *);
  mutable std::unique_ptr<FILE, FileDeleter> source{nullptr, &std::fclose};
  mutable int currentChar;
  mutable bool hasPeek;
  std::string filename;
};

} // namespace Bencode_Lib

#endif // BENCODE_ENABLE_FILE_IO