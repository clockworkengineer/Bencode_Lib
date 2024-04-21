//
// Unit Tests: Bencode_Lib_Tests_Encode
//
// Description: Encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("Bencode for encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode;
  SECTION("Encode an integer (266).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i266e");
  }
  SECTION("Encode an integer (10000).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i10000e");
  }
  SECTION("Encode an integer (-10000).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i-10000e");
  }
  SECTION("Encode an string ('qwertyuiopas').", "[Bencode][Encode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "12:qwertyuiopas");
  }
  SECTION("Encode an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "26:abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Encode an empty string ('').", "[Bencode][Encode][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "0:");
  }
}
TEST_CASE("Bencode for encode of a table of integer test data",
          "[Bencode][Encode][Integer]") {
  const Bencode bEncode;
  auto [expected] = GENERATE(table<std::string>({"i277e", "i32767e"}));
  BufferSource source(expected);
  BufferDestination destination;
  bEncode.decode(source);
  bEncode.encode(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for encode of a table of string test data",
          "[Bencode][Encode][String]") {
  const Bencode bEncode;
  auto [expected] =
      GENERATE(table<std::string>({"13:qwertyuiopasd", "6:mnbvcx"}));
  BufferSource source(expected);
  BufferDestination destination;
  bEncode.decode(source);
  bEncode.encode(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for encode of collection types (list, dictionary) ",
          "[Bencode][Encode]") {
  const Bencode bEncode;
  SECTION("Encode an List of integers('li266ei6780ei88ee').",
          "[Bencode][Encode][List]") {
    std::string expected{"li266ei6780ei88ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Encode an List of strings "
          "('l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze')."
          "[Bencode][Encode][List]") {
    std::string expected{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Encode ampty List."
          "[Bencode][Encode][List]") {
    std::string expected{"le"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Encode an Dictionary of integers.",
          "[Bencode][Encode][Dictionary]") {
    std::string expected{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Encode an Dictionary of strings.", "[Bencode][Encode][Dictionary]") {
    std::string expected{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Encode an empty  Dictionary.", "[Bencode][Encode][Dictionary]") {
    std::string expected{"de"};
    BufferSource source(expected);
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == expected);
  }
}
TEST_CASE("Encode generated exceptions", "[Bencode][Encode][Exceptions]") {
  const Bencode bEncode;
  SECTION("Encode called with no BNode tree to encode",
          "[Bencode][Encode][Exceptions]") {
    BufferDestination destination;
    REQUIRE_THROWS_AS(bEncode.encode(destination), Error);
    REQUIRE_THROWS_WITH(bEncode.encode(destination),
                        "Bencode Error: No Bencoded data to encode.");
  }
}
TEST_CASE("Encode torrent files using encodeToFile",
          "[Bencode][Encode][Torrents]") {
  const Bencode bEncode;
  SECTION("Encode singlefile.torrent.", "[Bencode][Encode][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kSingleFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Encode multifile.torrent.", "[Bencode][Encode][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}