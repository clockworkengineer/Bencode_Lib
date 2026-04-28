// File: Bencode_File_POSIX.cpp
//
// Description: POSIX-compatible file open functions used by the Bencode file I/O wrapper.

#include <cstdio>
#include <string>

namespace Bencode_Lib {

bool openBencodeFileForRead(const std::string &path, FILE *&file) {
  file = std::fopen(path.c_str(), "rb");
  return file != nullptr;
}

bool openBencodeFileForWrite(const std::string &path, FILE *&file) {
  file = std::fopen(path.c_str(), "wb");
  return file != nullptr;
}

} // namespace Bencode_Lib
