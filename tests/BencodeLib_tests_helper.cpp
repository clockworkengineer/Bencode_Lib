//
// Unit Tests: Bencode
//
// Description: Bencode unit test helper functions.
//
// ================
// Test definitions
// ================
#include "BencodeLib_Tests.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ==========================
// Unit test helper functions
// ==========================
/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file)
{
  std::filesystem::path currentPath = std::filesystem::current_path() / "testData" / file;
  return (currentPath.string());
}
/// <summary>
/// Compare two Bencoded files.
/// </summary>
/// <param name="fileName1">Bencoded file 1</param>
/// <param name="fileName2">Bencoded file 2</param>
/// <returns>true if files the same, false otherwise</returns>
bool compareFiles(const std::string &fileName1, const std::string &fileName2)
{
  std::ifstream file1(fileName1, std::ifstream::binary | std::ifstream::ate);
  std::ifstream file2(fileName2, std::ifstream::binary | std::ifstream::ate);
  if (file1.fail() || file2.fail())
  {
    return false; // file problem
  }
  if (file1.tellg() != file2.tellg())
  {
    return false; // size mismatch
  }
  // seek back to beginning and use std::equal to compare contents
  file1.seekg(0, std::ifstream::beg);
  file2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(file1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(file2.rdbuf()));
}
/// <summary>
/// Open a Bencoded file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="bencodedFileName">Bencoded file name</param>
/// <returns></returns>
std::string readBencodedBytesFromFile(const std::string &bencodedFileName)
{
  std::ifstream bencodedFile;
  bencodedFile.open(bencodedFileName, std::ifstream::binary);
  std::ostringstream bencodedFileBuffer;
  bencodedFileBuffer << bencodedFile.rdbuf();
  bencodedFile.close();
  return (bencodedFileBuffer.str());
}
/// <summary>
/// Convert byte buffer to a string.
/// the buffer.
/// </summary>
/// <param name="buffer">Bencoded byte buffer.</param>
/// <returns></returns>
std::string bufferToString(const std::vector<std::byte> &buffer)
{
    std::string destination;
    for (auto ch : buffer) {
        destination.push_back(static_cast<char>(ch));
    }
    return( destination);
}