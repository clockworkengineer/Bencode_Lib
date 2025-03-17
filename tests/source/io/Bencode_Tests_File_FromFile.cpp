#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Checks for fromFile() api.", "[Bencode][FromFile]")
{
  SECTION("Check that fromFile() works.", "[Bencode][FromFile][UTF8]")
  {
    const std::string testFile{ prefixTestDataPath("testfile001.ben") };
    std::string expected{ R"(d7:meaningi42e4:wiki7:bencodee)" };
    REQUIRE(Bencode::fromFile(testFile) == expected);
  }
}
