#include "Bencode_Lib_Tests.hpp"

TEST_CASE("IDestination (File interface).", "[Bencode][IDestination]") {
  SECTION("Create FileDestination.", "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    REQUIRE_NOTHROW(FileDestination(prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Create FileDestination when file already exists.",
          "[Bencode][IDestination]") {
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_NOTHROW(FileDestination(file.getFileName()));
  }
  SECTION("Create FileDestination and test file exists and should be empty.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    REQUIRE_FALSE(
        !std::filesystem::exists(file.getFileName()));
    std::filesystem::path filePath{file.getFileName()};
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 0);
  }
  SECTION("Create FileDestination and add one character.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add('i');
    file.close();
    std::filesystem::path filePath{file.getFileName()};
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 1);
  }
  SECTION("Create FileDestination, add an encoded integer.",
          "[Bencode][IDestination]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileDestination file{prefixTestDataPath(kGeneratedTorrentFile)};
    file.add("i65767e");
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 7);
    std::string expected{
        readBencodedBytesFromFile(file.getFileName())};
    REQUIRE(expected == "i65767e");
  }
}