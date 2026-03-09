#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Checks for fromFile() api.", "[Bencode][FromFile]") {
  SECTION("Check that fromFile() works.", "[Bencode][FromFile][UTF8]") {
    const std::string testFile{prefixTestDataPath("testfile001.ben")};
    std::string expected{R"(d7:meaningi42e4:wiki7:bencodee)"};
    REQUIRE(Bencode::fromFile(testFile) == expected);
  }
  SECTION("fromFile() returns a non-empty string for a known torrent file.",
          "[Bencode][FromFile]") {
    const std::string content{
        Bencode::fromFile(prefixTestDataPath(kSingleFileTorrent))};
    REQUIRE_FALSE(content.empty());
  }
  SECTION("fromFile() content from singlefile.torrent starts with 'd'.",
          "[Bencode][FromFile]") {
    const std::string content{
        Bencode::fromFile(prefixTestDataPath(kSingleFileTorrent))};
    REQUIRE(content.front() == 'd');
    REQUIRE(content.back() == 'e');
  }
  SECTION("fromFile() returns exactly 764 bytes for singlefile.torrent.",
          "[Bencode][FromFile]") {
    const std::string content{
        Bencode::fromFile(prefixTestDataPath(kSingleFileTorrent))};
    REQUIRE(content.size() == 764);
  }
  SECTION("fromFile() content is parseable by Bencode for singlefile.torrent.",
          "[Bencode][FromFile]") {
    const std::string content{
        Bencode::fromFile(prefixTestDataPath(kSingleFileTorrent))};
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{content}));
    REQUIRE(isA<Dictionary>(bencode.root()));
  }
  SECTION("fromFile() content is parseable by Bencode for multifile.torrent.",
          "[Bencode][FromFile]") {
    const std::string content{
        Bencode::fromFile(prefixTestDataPath(kMultiFileTorrent))};
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{content}));
    REQUIRE(isA<Dictionary>(bencode.root()));
  }
  SECTION("fromFile() round-trips with toFile() for a built Bencode structure.",
          "[Bencode][FromFile]") {
    Bencode bencode;
    bencode["level"] = 5;
    bencode["name"] = "player";
    BufferDestination dst;
    bencode.stringify(dst);
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, dst.toString());
    REQUIRE(Bencode::fromFile(testFile) == dst.toString());
    std::filesystem::remove(testFile);
  }
  SECTION("fromFile() of a file written by toFile() is parseable.",
          "[Bencode][FromFile]") {
    const std::string testFile{generateRandomFileName()};
    Bencode::toFile(testFile, "d3:keyi99ee");
    const std::string content{Bencode::fromFile(testFile)};
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{content}));
    REQUIRE(NRef<Integer>(bencode["key"]).value() == 99);
    std::filesystem::remove(testFile);
  }
}
