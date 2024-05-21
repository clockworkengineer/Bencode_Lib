#pragma once

#include <fstream>
#include <string>

#include "ISource.hpp"

namespace Bencode_Lib {

class FileSource final : public ISource {

public:
  // Constructors/Destructors
  explicit FileSource(const std::string &sourceFileName) {
    source.open(sourceFileName.c_str(), std::ios_base::binary);
    if (!source.is_open()) {
      throw Error(
          "Bencode file input stream failed to open or does not exist.");
    }
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  ~FileSource() override = default;

  char current() const override { return static_cast<char>(source.peek()); }
  void next() override {
    char c = 0;
    source.get(c);
  }
  bool more() const override { return source.peek() != EOF; }
  void reset() override {
    source.clear();
    source.seekg(0, std::ios_base::beg);
  }

private:
  mutable std::ifstream source;
};

} // namespace Bencode_Lib