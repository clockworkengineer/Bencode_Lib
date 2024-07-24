#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for decode of a table of integer test data",
          "[Bencode][Decode][Integer]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(
      table<std::string, int64_t>({{"i277e", 277}, {"i32767e", 32767}}));
  BufferSource source{testInput};
  bEncode.decode(source);
  REQUIRE(BRef<Integer>(bEncode.root()).value() == expected);
}
TEST_CASE("Bencode for decode of a table of string test data",
          "[Bencode][Decode][String]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>(
      {{"13:qwertyuiopasd", "qwertyuiopasd"}, {"6:mnbvcx", "mnbvcx"}}));
  BufferSource source{testInput};
  bEncode.decode(source);
  REQUIRE(BRef<String>(bEncode.root()).value() == expected);
}
TEST_CASE("Decode torrent files", "[Bencode][Decode][Torrents]") {
  const Bencode bEncode;
  SECTION("Decode singlefile.torrent", "[Bencode][Decode][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    bEncode.decode(source);
    REQUIRE(isA<Dictionary>(bEncode.root()));
  }
  SECTION("Decode singlefile.torrent. ", "[Bencode][Decode][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Decode multifile.torrent", "[Bencode][Decode][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    bEncode.decode(source);
    REQUIRE_FALSE(!isA<Dictionary>(bEncode.root()));
  }
  SECTION("Decode multifile.torrent. ", "[Bencode][Decode][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent)));
  }
}
TEST_CASE("Decode erroneous torrent files",
          "[Bencode][Decode][Torrents][Error]") {
  const Bencode bEncode;
  SECTION("Decode singlefileerror.torrent",
          "[Bencode][Decode][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kSingleFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode multifileerror.torrent",
          "[Bencode][Decode][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
}