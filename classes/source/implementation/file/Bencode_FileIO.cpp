
//
// Class: Bencode_Impl_FileIO
//
// Description: Public wrapper implementation for Bencode file-based I/O.
//              Platform-specific file open behavior is selected by CMake.
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode_Impl.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace Bencode_Lib {

bool openBencodeFileForRead(const std::string &path, FILE *&file);
bool openBencodeFileForWrite(const std::string &path, FILE *&file);

static void writeBencodeString(FILE *bencodeFile,
                               const std::string_view bencodeString) {
  if (!bencodeFile) {
    throw Error("File stream is not open for writing.");
  }
  std::size_t written =
      std::fwrite(bencodeString.data(), 1, bencodeString.size(), bencodeFile);
  if (written != bencodeString.size()) {
    throw Error("Failed to write Bencode string to file.");
  }
}

static std::string readBencodeString(FILE *bencodeFile) {
  if (!bencodeFile) {
    throw Error("File stream is not open for reading.");
  }

  std::vector<char> buffer;
  char chunk[4096];
  while (true) {
    std::size_t count = std::fread(chunk, 1, sizeof(chunk), bencodeFile);
    if (count > 0) {
      buffer.insert(buffer.end(), chunk, chunk + count);
    }
    if (count < sizeof(chunk)) {
      if (std::ferror(bencodeFile)) {
        throw Error("Failed to read Bencode file.");
      }
      break;
    }
  }
  return std::string(buffer.begin(), buffer.end());
}

std::string Bencode_Impl::fromFile(const std::string_view &fileName) {
  std::string path(fileName);
  FILE *file = nullptr;
  if (!openBencodeFileForRead(path, file) || !file) {
    throw Error("Bencode file input stream failed to open or does not exist.");
  }

  std::string result = readBencodeString(file);
  std::fclose(file);
  return result;
}

void Bencode_Impl::toFile(const std::string_view &fileName,
                          const std::string_view &bencodeString) {
  std::string path(fileName);
  FILE *file = nullptr;
  if (!openBencodeFileForWrite(path, file) || !file) {
    throw Error(
        "Bencode file output stream failed to open or could not be created.");
  }

  writeBencodeString(file, bencodeString);
  std::fclose(file);
}

} // namespace Bencode_Lib
