//
// Unit Tests: Bencode
//
// Description: ISource, IDestination and BNode unit tests for Bencode class
// using the Catch2 test framework.
//
// ================
// Test definitions
// ================
#include "BencodeLib_Tests.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ==========
// Test cases
// ==========
TEST_CASE("ISource (Buffer interface). Contains file singlefile.torrent.", "[Bencode][ISource]")
{
  std::string bencodedBuffer{readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent))};
  SECTION("Create BufferSource.", "[Bencode][ISource]")
  {
    REQUIRE_NOTHROW(BufferSource(bencodedBuffer));
  }
  SECTION("Create BufferSource with empty buffer.", "[Bencode][ISource]")
  {
    REQUIRE_THROWS_AS(BufferSource(""), std::invalid_argument);
    REQUIRE_THROWS_WITH(BufferSource(""), "Empty source buffer passed to be decoded.");
  }
  SECTION("Create BufferSource with singlefile.torrent and that it is positioned on the correct first character", "[Bencode][ISource]")
  {
    BufferSource source{bencodedBuffer};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create BufferSource with singlefile.torrent and then check moveToNextByte positions to correct next character", "[Bencode][ISource]")
  {
    BufferSource source{bencodedBuffer};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create BufferSource with singlefile.torrent move past last character, check it and the bytes moved.", "[Bencode][ISource]")
  {
    BufferSource source{bencodedBuffer};
    int64_t length = 0;
    while (source.more())
    {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                              // eof
    REQUIRE(source.current() == static_cast<char>(255)); // eof
  }
}
TEST_CASE("ISource (File interface).", "[Bencode][ISource]")
{
  SECTION("Create FileSource with singlefile.torrent.", "[Bencode][ISource]")
  {
    REQUIRE_NOTHROW(FileSource(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Create FileSource with non existants file.", "[Bencode][ISource]")
  {
    REQUIRE_THROWS_AS(FileSource(prefixTestDataPath(kNonExistantTorrent)), std::runtime_error);
    REQUIRE_THROWS_WITH(FileSource(prefixTestDataPath(kNonExistantTorrent)), "Bencode file input stream failed to open or does not exist.");
  }
  SECTION("Create FileSource with singlefile.torrent. and positioned on the correct first character", "[Bencode][ISource]")
  {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create FileSource with singlefile.torrent and then check moveToNextByte positions to correct next character", "[Bencode][ISource]")
  {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create FileSource with singlefile.torrent move past last character, check it and the bytes moved.", "[Bencode][ISource]")
  {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    int64_t length = 0;
    while (source.more())
    {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                               // eof
    REQUIRE(source.current() == static_cast<char>(0xff)); // eof
  }
}
TEST_CASE("IDestination (Buffer interface).", "[Bencode][IDestination]")
{
  SECTION("Create BufferDesination.", "[Bencode][IDestination]")
  {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.", "[Bencode][IDestination]")
  {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.getBuffer().empty());
  }
  SECTION("Create BufferDestination and add one character.", "[Bencode][IDestination]")
  {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.getBuffer().size() == 1);
  }
  SECTION("Create BufferDestination and add an encoded integer and check result.", "[Bencode][IDestination]")
  {
    BufferDestination buffer;
    buffer.add("i65767e");
    REQUIRE(buffer.getBuffer().size() == 7);
    REQUIRE(bufferToString(buffer.getBuffer()) == "i65767e");
  }
}
TEST_CASE("IDestination (File interface).", "[Bencode][IDestination]")
{
  SECTION("Create FileDestination.", "[Bencode][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination when file already exists.", "[Bencode][IDestination]")
  {
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination and test file exists and should be empty.", "[Bencode][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_FALSE(!std::filesystem::exists(prefixTestDataPath(kGeneratedTorrentFile)));
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.", "[Bencode][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add('i');
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an encoded integer and check result.", "[Bencode][IDestination]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add("i65767e");
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 7);
    std::string expected{readBencodedBytesFromFile(prefixTestDataPath(kGeneratedTorrentFile))};
    REQUIRE(expected == "i65767e");
  }
}
TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]")
{
  Bencode bEncode;
  SECTION("Decode dictionary and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())["one"]).string() == "0123456789");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())["two"]).string() == "asdfghjkl");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())["three"]).string() == "qwerty");
  }
  SECTION("Decode list and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource{"l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[0]).string() == "sillyy");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[1]).string() == "poiuytrewqas");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[2]).string() == "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode list with embedded dictioanry and check its components using indexing", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource{"l6:sillyyd3:one10:01234567895:three6:qwerty3:two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[0]).string() == "sillyy");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[1]["one"]).string() == "0123456789");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[1]["two"]).string() == "asdfghjkl");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[1]["three"]).string() == "qwerty");
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())[2]).string() == "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode dictionary and check an invalid key generates exception", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())["onee"].nodeType == BNodeType::dictionary, BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())["onee"].nodeType == BNodeType::dictionary, "BNode Error: Invalid key used in dictionary.");
  }
  SECTION("Decode list and check an invalid index generates exception", "[Bencode][BNode][Index]")
  {
    BufferSource bEncodeSource{"l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())[3].nodeType == BNodeType::list, BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())[3].nodeType == BNodeType::list, "BNode Error: Invalid index used in list.");
  }
}
TEST_CASE("Check BNode reference functions work.", "[Bencode][BNode][Reference]")
{
  Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource{"i45500e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeInteger>((bEncode.root())).integer() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource{"10:0123456789"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>((bEncode.root())).string() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource{"l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeList>((bEncode.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]")
  {
    BufferSource bEncodeSource{"d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeDict>((bEncode.root())).size() == 4);
  }
}
TEST_CASE("Check R-Value reference encode/decode.", "[Bencode][BNode][Reference]")
{
  Bencode bEncode;
  SECTION("Encode/Decode with R-Value reference (Buffer).", "[Bencode][BNode][R-Value Reference]")
  {
    bEncode.decode(BufferSource{"i45500e"});
    bEncode.encode(BufferDestination{}); // Does nothing as sink (for completeness)
    REQUIRE(BNodeRef<BNodeInteger>((bEncode.root())).integer() == 45500);
  }
  SECTION("Encode/Decode both with R-Value reference (File).", "[Bencode][BNode][R-alue Reference]")
  {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    bEncode.decode(FileSource{prefixTestDataPath(kMultiFileTorrent)});
    bEncode.encode(FileDestination{prefixTestDataPath(kGeneratedTorrentFile)});
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent), prefixTestDataPath(kGeneratedTorrentFile)));
  }
}