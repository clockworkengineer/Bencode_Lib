#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Checks for ToFile() api.", "[Bencode][ToFile]") {
  SECTION("Check that ToFile() works.", "[Bencode][Tofile]]") {
    const std::string testFile{generateRandomFileName()};
    std::string expected{R"(d7:meaningi42e4:wiki7:bencodee)"};
    Bencode::toFile(testFile, expected);
    REQUIRE(Bencode::fromFile(testFile) == expected);
  }
  SECTION("toFile() creates the file on disk.", "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, "i1e");
    REQUIRE(std::filesystem::exists(testFile));
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() writes the correct number of bytes.", "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    const std::string content{"li1ei2ei3ee"};
    Bencode::toFile(testFile, content);
    REQUIRE(std::filesystem::file_size(testFile) == content.size());
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() with an empty string creates an empty file.",
          "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, "");
    REQUIRE(std::filesystem::exists(testFile));
    REQUIRE(std::filesystem::file_size(testFile) == 0);
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() overwrites existing file content.", "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, "i1e");
    Bencode::toFile(testFile, "i999e");
    REQUIRE(Bencode::fromFile(testFile) == "i999e");
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() round-trips a list encoding correctly.",
          "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    const std::string content{"li10ei20ei30ee"};
    Bencode::toFile(testFile, content);
    REQUIRE(Bencode::fromFile(testFile) == content);
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() round-trips a nested dictionary encoding correctly.",
          "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    const std::string content{R"(d6:answerd10:everythingi42ee4:name5:Nielse)"};
    Bencode::toFile(testFile, content);
    REQUIRE(Bencode::fromFile(testFile) == content);
    std::filesystem::remove(testFile);
  }
  SECTION("Stringify to buffer then toFile() and parse back matches values.",
          "[Bencode][ToFile]") {
    Bencode bencode;
    bencode["score"] = 100;
    bencode["tag"] = "test";
    BufferDestination dst;
    bencode.stringify(dst);
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, dst.toString());
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(FileSource{testFile}));
    REQUIRE(NRef<Integer>(bencode2["score"]).value() == 100);
    REQUIRE(NRef<String>(bencode2["tag"]).value() == "test");
    std::filesystem::remove(testFile);
  }
  SECTION("toFile() written content can be parsed by Bencode.",
          "[Bencode][ToFile]") {
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, "d3:keyi7ee");
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(FileSource{testFile}));
    REQUIRE(NRef<Integer>(bencode["key"]).value() == 7);
    std::filesystem::remove(testFile);
  }
}
