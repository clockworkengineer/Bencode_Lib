#include "Bencode_Lib_Tests.hpp"

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
  SECTION("Create FileDestination, add an encoded integer.",
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