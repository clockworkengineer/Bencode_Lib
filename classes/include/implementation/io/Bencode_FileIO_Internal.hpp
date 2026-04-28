// File: Bencode_FileIO_Internal.hpp
//
// Description: Shared file I/O helper declarations used by the Bencode file I/O
// wrapper and platform-specific adapters.

#pragma once

#include <cstdio>
#include <string>
#include <string_view>

namespace Bencode_Lib {

bool openBencodeFileForRead(const std::string &path, FILE *&file);
bool openBencodeFileForWrite(const std::string &path, FILE *&file);

class Bencode_FileHandle {
public:
  enum class Mode { Read, Write };

  explicit Bencode_FileHandle(const std::string_view &path, Mode mode);
  ~Bencode_FileHandle();

  Bencode_FileHandle(const Bencode_FileHandle &) = delete;
  Bencode_FileHandle &operator=(const Bencode_FileHandle &) = delete;

  Bencode_FileHandle(Bencode_FileHandle &&other) noexcept;
  Bencode_FileHandle &operator=(Bencode_FileHandle &&other) noexcept;

  bool isOpen() const noexcept;
  FILE *get() const noexcept;
  operator FILE *() const noexcept;

private:
  FILE *file_;
};

std::string readBencodeString(FILE *bencodeFile);
void writeBencodeString(FILE *bencodeFile,
                        const std::string_view bencodeString);

} // namespace Bencode_Lib
