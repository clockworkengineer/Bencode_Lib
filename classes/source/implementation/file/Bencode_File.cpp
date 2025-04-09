
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

namespace Bencode_Lib {

/// <summary>
/// Write Bencode string to a file stream.
/// </summary>
/// <param name="bencodeFile">Bencode file stream</param>
/// <param name="bencodeString">Bencode string</param>
void writeBencodeString(std::ofstream &bencodeFile, const std::string &bencodeString) { bencodeFile << bencodeString; }
/// <summary>
/// Read Bencode string from a file stream.
/// </summary>
/// <param name="bencodeFile">Bencode file stream</param>
/// <returns>Bencode string.</returns>
std::string readBencodeString(const std::ifstream &bencodeFile)
{
  std::ostringstream bencodeFileBuffer;
  bencodeFileBuffer << bencodeFile.rdbuf();
  return bencodeFileBuffer.str();
}
/// <summary>
/// Open a Bencode file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <returns>Bencode string.</returns>
std::string Bencode_Impl::fromFile(const std::string &fileName)
{
  // Read in Bencode
  return readBencodeString(std::ifstream{ fileName, std::ios_base::binary });
}
/// <summary>
/// Create a Bencode file and write Bencode string to it.
/// </summary>
/// <param name="fileName">Bencode file name</param>
/// <param name="bencodeString">Bencode string</param>
void Bencode_Impl::toFile(const std::string &fileName, const std::string &bencodeString)
{
  std::ofstream bencodeFile{ fileName, std::ios::binary };
  writeBencodeString(bencodeFile, bencodeString);
  bencodeFile.close();
}
}// namespace Bencode_Lib

