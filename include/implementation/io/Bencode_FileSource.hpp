#pragma once

#include <fstream>
#include <string>

#include "ISource.hpp"

namespace Bencode_Lib {

class FileSource : public ISource {
public:
  explicit FileSource(const std::string &sourceFileName) {
    m_source.open(sourceFileName.c_str(), std::ios_base::binary);
    if (!m_source.is_open()) {
      throw Error(
          "Bencode file input stream failed to open or does not exist.");
    }
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  virtual ~FileSource() = default;

  char current() const override { return (static_cast<char>(m_source.peek())); }
  void next() override {
    char c = 0;
    m_source.get(c);
  }
  bool more() const override { return (m_source.peek() != EOF); }
  void reset() override {
    m_source.clear();
    m_source.seekg(0, std::ios_base::beg);
  }

private:
  mutable std::ifstream m_source;
};
} // namespace Bencode_Lib