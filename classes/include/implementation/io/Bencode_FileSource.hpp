#pragma once

#if BENCODE_ENABLE_FILE_IO

#include "ISource.hpp"

#include <cstdio>
#include <string>
#include <string_view>

namespace Bencode_Lib {

class FileSource final : public ISource {

public:
  // Constructors/Destructors
  explicit FileSource(const std::string_view &sourceFileName)
      : source(nullptr), currentChar(EOF), hasPeek(false),
        filename(sourceFileName) {
#ifdef _MSC_VER
    if (fopen_s(&source, filename.c_str(), "rb") != 0 || !source) {
#else
    source = std::fopen(filename.c_str(), "rb");
    if (!source) {
#endif
      throw Error(
          "Bencode file input stream failed to open or does not exist.");
    }
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  ~FileSource() override { close(); }

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
    std::fgetc(source);
    hasPeek = false;
  }
  bool more() const override {
    ensurePeek();
    return currentChar != EOF;
  }
  void reset() override {
    if (source) {
      std::rewind(source);
      hasPeek = false;
      currentChar = EOF;
    }
  }
  std::string getFileName() { return filename; }
  void close() {
    if (source) {
      std::fclose(source);
      source = nullptr;
      hasPeek = false;
      currentChar = EOF;
    }
  }

private:
  void ensurePeek() const {
    if (hasPeek || !source) {
      return;
    }
    currentChar = std::fgetc(source);
    if (currentChar != EOF) {
      std::ungetc(currentChar, source);
    }
    hasPeek = true;
  }

  mutable FILE *source;
  mutable int currentChar;
  mutable bool hasPeek;
  std::string filename;
};

} // namespace Bencode_Lib

#endif // BENCODE_ENABLE_FILE_IO