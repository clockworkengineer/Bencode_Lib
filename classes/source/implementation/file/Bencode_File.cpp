
//
// Class: Bencode_Impl_File
//
// Description: Bencode class implementation layer to read and write
// Bencode files (which are binary format files).
//
// Dependencies: C++20 - Language standard features used.
//

#include "Bencode.hpp"
#include "Bencode_Impl.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace Bencode_Lib {

/// <summary>
/// Write Bencode string to a file stream.
/// </summary>
/// <param name="bencodeFile">Bencode file stream</param>
/// <param name="bencodeString">Bencode string</param>
void writeBencodeString(FILE *bencodeFile,
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
/// <summary>
/// Read Bencode string from a file stream.
/// </summary>
/// <param name="bencodeFile">Bencode file stream.</param>
/// <returns>Bencode string.</returns>
std::string readBencodeString(FILE *bencodeFile) {
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
/// <summary>
/// Open a Bencode file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <returns>Bencode string.</returns>
std::string Bencode_Impl::fromFile(const std::string_view &fileName) {
  std::string path(fileName);
  FILE *file = nullptr;
#ifdef _MSC_VER
  if (fopen_s(&file, path.c_str(), "rb") != 0 || !file) {
#else
  file = std::fopen(path.c_str(), "rb");
  if (!file) {
#endif
    throw Error("Bencode file input stream failed to open or does not exist.");
  }
  std::string result = readBencodeString(file);
  std::fclose(file);
  return result;
}
/// <summary>
/// Create a Bencode file and write Bencode string to it.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <param name="bencodeString">Bencode string</param>
void Bencode_Impl::toFile(const std::string_view &fileName,
                          const std::string_view &bencodeString) {
  std::string path(fileName);
  FILE *file = nullptr;
#ifdef _MSC_VER
  if (fopen_s(&file, path.c_str(), "wb") != 0 || !file) {
#else
  file = std::fopen(path.c_str(), "wb");
  if (!file) {
#endif
    throw Error(
        "Bencode file output stream failed to open or could not be created.");
  }
  writeBencodeString(file, bencodeString);
  std::fclose(file);
}
} // namespace Bencode_Lib
