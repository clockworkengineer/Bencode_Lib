#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of a table of integer test data",
          "[Bencode][Parse][Integer]") {
  const Bencode bStringify;
  auto [testInput, expected] = GENERATE(
      table<std::string, int64_t>({{"i277e", 277}, {"i32767e", 32767}}));
  BufferSource source{testInput};
  bStringify.parse(source);
  REQUIRE(BRef<Integer>(bStringify.root()).value() == expected);
}
TEST_CASE("Bencode for parse of a table of string test data",
          "[Bencode][Parse][String]") {
  const Bencode bStringify;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>(
      {{"13:qwertyuiopasd", "qwertyuiopasd"}, {"6:mnbvcx", "mnbvcx"}}));
  BufferSource source{testInput};
  bStringify.parse(source);
  REQUIRE(BRef<String>(bStringify.root()).value() == expected);
}
TEST_CASE("Parse torrent files", "[Bencode][Parse][Torrents]") {
  const Bencode bStringify;
  SECTION("Parse singlefile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    bStringify.parse(source);
    REQUIRE(isA<Dictionary>(bStringify.root()));
  }
  SECTION("Parse singlefile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Parse multifile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<Dictionary>(bStringify.root()));
  }
  SECTION("Parse multifile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent)));
  }
}
TEST_CASE("Parse erroneous torrent files",
          "[Bencode][Parse][Torrents][Error]") {
  const Bencode bStringify;
  SECTION("Parse singlefileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kSingleFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse multifileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
}