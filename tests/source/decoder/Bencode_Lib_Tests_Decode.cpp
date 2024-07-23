//
// Unit Tests: Bencode_Lib_Tests_Decode
//
// Description: Decode unit tests for Bencode class
// using the Catch2 test framework.
//

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
TEST_CASE("Decode generated exceptions", "[Bencode][Decode][Exceptions]") {
  const Bencode bEncode;
  SECTION("Decode an string without terminating ':' on its length",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"26abcdefghijklmnopqrstuvwxyz"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an integer without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"i266"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an list without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"li266ei6780ei88e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an dictionary without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an string that terminates prematurely",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"26:abcdefghijklmno"};
    REQUIRE_THROWS_WITH(
        bEncode.decode(source),
        "ISource Error: Decode buffer empty before decode complete.");
  }
  SECTION("Duplicate dictionary keys",
          "[Bencode][Decode][Dictionary][Exceptions]") {
    BufferSource source{"d3:one10:01234567893:two6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Dictionary Keys not in lexical order",
          "[Bencode][Decode][Dictionary][Exceptions]") {
    BufferSource source{"d5:three10:01234567893:one6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an Dictionary of ints that terminates early",
          "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3ee3:twoi2ee"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
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