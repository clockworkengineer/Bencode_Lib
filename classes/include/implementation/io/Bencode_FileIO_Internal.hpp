// File: Bencode_FileIO_Internal.hpp
//
// Description: Shared file I/O helper declarations used by the Bencode file I/O
// wrapper and platform-specific adapters.

#pragma once

#include <cstdio>
#include <string>

namespace Bencode_Lib {

bool openBencodeFileForRead(const std::string &path, FILE *&file);
bool openBencodeFileForWrite(const std::string &path, FILE *&file);

} // namespace Bencode_Lib
