//
// Unit Tests: Bencode_Lib_Tests_IDestination
//
// Description: IDestination unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("IDestination (Buffer interface).", "[Bencode][IDestination]") {
  SECTION("Create BufferDestinations.", "[Bencode][IDestination]") {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    REQUIRE_FALSE(!buffer.size()==0);
  }
  SECTION("Create BufferDestination and add one character.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.size() == 1);
  }
  SECTION(
      "Create BufferDestination and add an encoded integer and check result.",
      "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i65767e");
    REQUIRE(buffer.size() == 7);
    REQUIRE(buffer.toString() == "i65767e");
  }
}
TEST_CASE("IDestination (File interface).", "[Bencode][IDestination]") {
  SECTION("Create FileDestination.", "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination when file already exists.",
          "[Bencode][IDestination]") {
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination and test file exists and should be empty.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_FALSE(
        !std::filesystem::exists(prefixTestDataPath(kGeneratedTorrentFile)));
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 0);
  }
  SECTION("Create FileDestination and add one character.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add('i');
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 1);
  }
  SECTION("Create FileDestination, add an encoded integer and check result.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add("i65767e");
    std::filesystem::path filePath{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE(std::filesystem::file_size(filePath) == 7);
    std::string expected{
        readBencodedBytesFromFile(prefixTestDataPath(kGeneratedTorrentFile))};
    REQUIRE(expected == "i65767e");
  }
}