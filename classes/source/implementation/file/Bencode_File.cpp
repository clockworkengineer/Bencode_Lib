
//
// Class: Bencode_Impl_File
//
// Description: Bencode class implementation layer to read and write
// Bencode files in a number of different formats. Note that these
// methods are all static and do not need a Bencode object to invoke.
// For more information on byte marks and their meaning then check out link
// https://en.wikipedia.org/wiki/Byte_order_mark.
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
  std::ifstream bencodeFile{ fileName, std::ios_base::binary };
  return readBencodeString(bencodeFile);
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

