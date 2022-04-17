//
// Unit Tests: Bencode
//
// Description: ISource, IDestination and BNode unit tests for Bencode class
// using the Catch2 test framework.
//
// ================
// Test definitions
// ================
#include "Bencode_tests.hpp"
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
// ==========
// Test cases
// ==========
TEST_CASE("ISource (File interface).", "[Bencode][Decode][ISource]")
{
  SECTION("Create FileSource with singlefile.torrent.", "[Bencode][Decode][ISource]")
  {
    REQUIRE_NOTHROW(FileSource(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Create FileSource with non existants file.", "[Bencode][Decode][ISource]")
  {
    REQUIRE_THROWS_AS(FileSource(prefixTestDataPath(kNonExistantTorrent)), std::runtime_error);
    REQUIRE_THROWS_WITH(FileSource(prefixTestDataPath(kNonExistantTorrent)), "Bencode file input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource with singlefile.torrent. and positioned on the correct first character", "[Bencode][Decode][ISource]")
  {
    FileSource source { prefixTestDataPath(kSingleFileTorrent) };
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create FileSource with singlefile.torrent and then check moveToNextByte positions to correct next character", "[Bencode][Decode][ISource]")
  {
    FileSource source { prefixTestDataPath(kSingleFileTorrent) };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create FileSource with singlefile.torrent move past last character, check it and the bytes moved.", "[Bencode][Decode][ISource]")
  {
    FileSource source { prefixTestDataPath(kSingleFileTorrent) };
    long length = 0;
    while (source.more())
    {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                               // eof
    REQUIRE(source.current() == static_cast<char>(0xff)); // eof
  }
}
TEST_CASE("ISource (Buffer interface). Contains file singlefile.torrent.", "[Bencode][Decode][ISource]")
{
  std::string bencodedBuffer { readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)) };
  SECTION("Create BufferSource.", "[Bencode][Decode][ISource]")
  {
    REQUIRE_NOTHROW(BufferSource(bencodedBuffer));
  }
  SECTION("Create BufferSource with empty buffer.", "[Bencode][Decode][ISource]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), std::invalid_argument);
    REQUIRE_THROWS_WITH(BufferSource(""), "Empty source buffer passed to be decoded.");
  }
  SECTION("Create BufferSource with singlefile.torrent and that it is positioned on the correct first character", "[Bencode][Decode][ISource]")
  {
    BufferSource source { bencodedBuffer };
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create BufferSource with singlefile.torrent and then check moveToNextByte positions to correct next character", "[Bencode][Decode][ISource]")
  {
    BufferSource source { bencodedBuffer };
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create BufferSource with singlefile.torrent move past last character, check it and the bytes moved.", "[Bencode][Decode][ISource]")
  {
    BufferSource source { bencodedBuffer };
    long length = 0;
    while (source.more())
    {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                              // eof
    REQUIRE(source.current() == static_cast<char>(255)); // eof
  }
}
TEST_CASE("IDestination (Buffer interface).", "[Bencode][Decode][ISource]")
{
  SECTION("Create BufferDesination.", "[Bencode][Encode][IDesination]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[Bencode][Encode][IDesination]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().isEmpty());
  }
  SECTION("Create BufferDestination and add one character.", "[Bencode][Encode][IDesination]")
  {
    BufferDestination buffer;
    buffer.add("i");
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION("Create BufferDestination and add an encoded integer and check result.", "[Bencode][Encode][IDesination]")
  {
    BufferDestination buffer;
    buffer.add("i65767e");
    REQUIRE(buffer.getBuffer().size() == 7);
    REQUIRE(buffer.getBuffer() == Bencoding("i65767e"));
  }
}
TEST_CASE("IDestination (File interface).", "[Bencode][Decode][ISource]")
{
  SECTION("Create FileDestination.", "[Bencode][Encode][IDesination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination when file already exists.", "[Bencode][Encode][IDesination]")
  {
    FileDestination file { prefixTestDataPath(kGeneratedTorrentFile) };
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[Bencode][Encode][IDesination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file { prefixTestDataPath(kGeneratedTorrentFile) };
    REQUIRE_FALSE(!std::filesystem::exists(prefixTestDataPath(kGeneratedTorrentFile)));
    std::filesystem::path filePath { prefixTestDataPath(kGeneratedTorrentFile) };
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[Bencode][Encode][IDesination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file { prefixTestDataPath(kGeneratedTorrentFile) };
    file.add("i");
    std::filesystem::path filePath { prefixTestDataPath(kGeneratedTorrentFile) };
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an encoded integer and check result.", "[Bencode][Encode][IDesination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file { prefixTestDataPath(kGeneratedTorrentFile) };
    file.add("i65767e");
    std::filesystem::path filePath { prefixTestDataPath(kGeneratedTorrentFile) };
    REQUIRE(std::filesystem::file_size(filePath) == 7);
    std::string expected  { readBencodedBytesFromFile(prefixTestDataPath(kGeneratedTorrentFile)) };
    REQUIRE(expected == "i65767e");
  }
}
TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]")
{
  Bencode bEncode;
  SECTION("Decode dictionary and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource { "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())["one"]).getString() == "0123456789");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())["two"]).getString() == "asdfghjkl");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())["three"]).getString() == "qwerty");
  }
  SECTION("Decode list and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource { "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[0]).getString() == "sillyy");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[1]).getString() == "poiuytrewqas");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[2]).getString() == "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode list with embedded dictioanry and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource { "l6:sillyyd3:one10:01234567895:three6:qwerty3:two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[0]).getString() == "sillyy");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[1]["one"]).getString() == "0123456789");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[1]["two"]).getString() == "asdfghjkl");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[1]["three"]).getString() == "qwerty");
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[2]).getString() == "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode dictionary and check an invalid key generates exception", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource { "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle" };
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((*bEncode.getRoot())["onee"].nodeType == BNodeType::dictionary, std::runtime_error);
    REQUIRE_THROWS_WITH((*bEncode.getRoot())["onee"].nodeType == BNodeType::dictionary, "Invalid key used in dictionary.");
  }
  SECTION("Decode list and check an invalid index generates exception", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource {"l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze" };
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((*bEncode.getRoot())[3].nodeType == BNodeType::list, std::runtime_error);
    REQUIRE_THROWS_WITH((*bEncode.getRoot())[3].nodeType == BNodeType::list, "Invalid index used in list.");
  }
}
TEST_CASE("Check BNode reference functions work.", "[Bencode][BNode][Reference]")
{
  Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource { "i45500e" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())).getInteger() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource { "10:0123456789" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())).getString() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource { "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeList>((*bEncode.getRoot())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource { "d3:one10:01234567894:four8:123456785:three6:qwerty3:two9:asdfghjkle" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeDict>((*bEncode.getRoot())).size() == 4);
  }
}