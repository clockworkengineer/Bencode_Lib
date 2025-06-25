#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Checks for ToFile() api.", "[Bencode][ToFile]") {
  SECTION("Check that ToFile() works.", "[Bencode][Tofile]]") {
    const std::string testFile{generateRandomFileName()};
    std::string expected{R"(d7:meaningi42e4:wiki7:bencodee)"};
    Bencode::toFile(testFile, expected);
    REQUIRE(Bencode::fromFile(testFile) == expected);
  }
}
