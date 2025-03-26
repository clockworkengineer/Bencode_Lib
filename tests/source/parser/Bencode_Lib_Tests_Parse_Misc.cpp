#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of a table of integer test data",
          "[Bencode][Parse][Integer]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(
      table<std::string, int64_t>({{"i277e", 277}, {"i32767e", 32767}}));
  BufferSource source{testInput};
  bEncode.parse(source);
  REQUIRE(BRef<Integer>(bEncode.root()).value() == expected);
}
TEST_CASE("Bencode for parse of a table of string test data",
          "[Bencode][Parse][String]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>(
      {{"13:qwertyuiopasd", "qwertyuiopasd"}, {"6:mnbvcx", "mnbvcx"}}));
  BufferSource source{testInput};
  bEncode.parse(source);
  REQUIRE(BRef<String>(bEncode.root()).value() == expected);
}
TEST_CASE("Parse torrent files", "[Bencode][Parse][Torrents]") {
  const Bencode bEncode;
  SECTION("Parse singlefile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    bEncode.parse(source);
    REQUIRE(isA<Dictionary>(bEncode.root()));
  }
  SECTION("Parse singlefile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Parse multifile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    bEncode.parse(source);
    REQUIRE_FALSE(!isA<Dictionary>(bEncode.root()));
  }
  SECTION("Parse multifile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent)));
  }
}
TEST_CASE("Parse erroneous torrent files",
          "[Bencode][Parse][Torrents][Error]") {
  const Bencode bEncode;
  SECTION("Parse singlefileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kSingleFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.parse(source), SyntaxError);
  }
  SECTION("Parse multifileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.parse(source), SyntaxError);
  }
}