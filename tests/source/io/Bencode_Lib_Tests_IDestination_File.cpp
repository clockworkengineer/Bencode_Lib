#include "Bencode_Lib_Tests.hpp"

TEST_CASE("IDestination (File interface).", "[Bencode][IDestination]") {
  SECTION("Create FileDestination.", "[Bencode][IDestination]") {
    std::string fileName{generateRandomFileName()};
    REQUIRE_NOTHROW(FileDestination(fileName));
    REQUIRE(std::filesystem::exists(fileName));
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination when file already exists.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.close(); // Ensure file is created.
    REQUIRE_NOTHROW(FileDestination(file.getFileName()));
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination and test file exists and should be empty.",
          "[Bencode][IDestination]") {

    FileDestination file{generateRandomFileName()};
    REQUIRE_FALSE(!std::filesystem::exists(file.getFileName()));
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 0);
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination and add one character.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add('i');
    file.close();
    std::filesystem::path filePath{file.getFileName()};
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 1);
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination, add an encoded integer.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i65767e");
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 7);
    std::string expected{readBencodedBytesFromFile(file.getFileName())};
    REQUIRE(expected == "i65767e");
    std::filesystem::remove(file.getFileName());
  }
}