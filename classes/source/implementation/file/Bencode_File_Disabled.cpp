// File: Bencode_File_Disabled.cpp
//
// Description: Stub source used when file-based I/O support is disabled.

#include "Bencode_Impl.hpp"

namespace Bencode_Lib {

std::string Bencode_Impl::fromFile(const std::string_view &fileName) {
  (void)fileName;
  throw Error("File-based I/O support is disabled.");
}

void Bencode_Impl::toFile(const std::string_view &fileName,
                          const std::string_view &bencodeString) {
  (void)fileName;
  (void)bencodeString;
  throw Error("File-based I/O support is disabled.");
}

} // namespace Bencode_Lib
