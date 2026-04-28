// File: Bencode_File_MSVC.cpp
//
// Description: MSVC-compatible file open functions used by the Bencode file I/O wrapper.

#include <cstdio>
#include <string>

namespace Bencode_Lib {

bool openBencodeFileForRead(const std::string &path, FILE *&file) {
  return fopen_s(&file, path.c_str(), "rb") == 0 && file != nullptr;
}

bool openBencodeFileForWrite(const std::string &path, FILE *&file) {
  return fopen_s(&file, path.c_str(), "wb") == 0 && file != nullptr;
}

} // namespace Bencode_Lib
