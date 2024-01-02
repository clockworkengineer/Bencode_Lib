//
// Unit Tests: Bencode
//
// Description: Decode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("Bencode for decode of simple types (integer, string) ",
          "[Bencode][Decode]") {
  const Bencode bEncode;
  SECTION("Decode an integer", "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i266e"};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isInteger());
  }
  SECTION("Decode an integer (266) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i266e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 266);
  }
  SECTION("Decode an integer (1000) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i1000e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 1000);
  }
  SECTION("Decode an integer (0) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i0e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 0);
  }
  SECTION("Decode an negative integer (-666) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i-666e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == -666);
  }
  SECTION("Decode an negative zero (-0) is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"i-0e"}), IDecoder::Error);
  }
  SECTION("Decode an integer with leading zeros is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"i0012e"}), IDecoder::Error);
  }
  SECTION("Decode an empty integer is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"ie"}), IDecoder::Error);
  }
  SECTION("Decode max 64 bit integer (9223372036854775807) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i9223372036854775807e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::max());
  }
  SECTION("Decode out of range positive 64 bit integer (9223372036854775808) "
          "and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i9223372036854775808e"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), std::out_of_range);
  }
  SECTION("Decode min 64 bit integer (-9223372036854775808) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i-9223372036854775808e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::min());
  }
  SECTION("Decode out of range negative 64 bit integer (-9223372036854775809) "
          "and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource bEncodeSource{"i-9223372036854775809e"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), std::out_of_range);
  }
  SECTION("Decode an string", "[Bencode][Decode][String]") {
    BufferSource bEncodeSource{"12:qwertyuiopas"};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isString());
  }
  SECTION("Decode a string ('qwertyuiopas') and check value",
          "[Bencode][Decode][String]") {
    BufferSource bEncodeSource{"12:qwertyuiopas"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>(bEncode.root()).value() == "qwertyuiopas");
  }
  SECTION("Decode a string ('abcdefghijklmnopqrstuvwxyz') and check value",
          "[Bencode][Decode][String]") {
    BufferSource bEncodeSource{"26:abcdefghijklmnopqrstuvwxyz"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>(bEncode.root()).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode a string with zero length", "[Bencode][Decode][String]") {
    REQUIRE_NOTHROW(bEncode.decode(BufferSource{"0:"}));
    REQUIRE(BRef<String>(bEncode.root()).value() == "");
  }
  SECTION("Decode a string with no length", "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{":"}), IDecoder::Error);
  }
  SECTION("Decode a string with negative length", "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"-2:ww"}), IDecoder::Error);
  }
  SECTION("Decode a string with max length (buffer overflow attempt)",
          "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"9223372036854775807:ww"}),
                      std::runtime_error);
  }
}
TEST_CASE("Bencode for decode of a table of integer test data",
          "[Bencode][Decode][Integer]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(
      table<std::string, int64_t>({{"i277e", 277}, {"i32767e", 32767}}));
  BufferSource bEncodeSource{testInput};
  bEncode.decode(bEncodeSource);
  REQUIRE(BRef<Integer>(bEncode.root()).value() == expected);
}
TEST_CASE("Bencode for decode of a table of string test data",
          "[Bencode][Decode][String]") {
  const Bencode bEncode;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>(
      {{"13:qwertyuiopasd", "qwertyuiopasd"}, {"6:mnbvcx", "mnbvcx"}}));
  BufferSource bEncodeSource{testInput};
  bEncode.decode(bEncodeSource);
  REQUIRE(BRef<String>(bEncode.root()).value() == expected);
}
TEST_CASE("Bencode for decode of collection types (list, dictionary) ",
          "[Bencode][Decode][List]") {
  const Bencode bEncode;
  SECTION("Decode an List", "[Bencode][Decode]") {
    BufferSource bEncodeSource{"li266ei6780ei88e5:threee"};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isList());
    REQUIRE(bEncode[0].isInteger());
    REQUIRE(bEncode[1].isInteger());
    REQUIRE(bEncode[2].isInteger());
    REQUIRE(bEncode[3].isString());
    REQUIRE(BRef<Integer>(bEncode[0]).value() == 266);
    REQUIRE(BRef<Integer>(bEncode[1]).value() == 6780);
    REQUIRE(BRef<Integer>(bEncode[2]).value() == 88);
    REQUIRE(BRef<String>(bEncode[3]).value() == "three");
  }
  SECTION("Decode an list of integers and check values",
          "[Bencode][Decode][List]") {
    BufferSource bEncodeSource{"li266ei6780ei88ee"};
    bEncode.decode(bEncodeSource);
    std::vector<int64_t> numbers;
    for (const auto &bNode : BRef<List>(bEncode.root()).value()) {
      numbers.push_back(BRef<Integer>(bNode).value());
    }
    REQUIRE(numbers == std::vector<int64_t>{266, 6780, 88});
  }
  SECTION("Decode an list of strings and check values",
          "[Bencode][Decode][List]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    std::vector<std::string> strings;
    for (const auto &bNode : BRef<List>(bEncode.root()).value()) {
      strings.push_back(BRef<String>(bNode).value());
    }
    REQUIRE(strings == std::vector<std::string>{"sillyy", "poiuytrewqas",
                                                "abcdefghijklmnopqrstuvwxyz"});
  }
  SECTION("Decode an Dictionary", "[Bencode][Decode][Dictionary]") {
    BufferSource bEncodeSource{"d3:onei1e5:threei3e3:twoi2ee"};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isDictionary());
    REQUIRE(bEncode["one"].isInteger());
    REQUIRE(bEncode["two"].isInteger());
    REQUIRE(bEncode["three"].isInteger());
    REQUIRE(BRef<Integer>(bEncode["one"]).value() == 1);
    REQUIRE(BRef<Integer>(bEncode["two"]).value() == 2);
    REQUIRE(BRef<Integer>(bEncode["three"]).value() == 3);
  }
  SECTION("Decode an Dictionary of ints and check values",
          "[Bencode][Decode][Dictionary]") {
    BufferSource bEncodeSource{"d3:onei1e5:threei3e3:twoi2ee"};
    bEncode.decode(bEncodeSource);
    std::map<std::string, int64_t> entries;
    for (const auto &bNode : BRef<Dictionary>(bEncode.root()).value()) {
      entries[bNode.getKey()] = BRef<Integer>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, int64_t>{
                           {"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Decode a Dictionary of strings and check values",
          "[Bencode][Decode][Dictionary]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    std::map<std::string, std::string> entries;
    for (const auto &bNode : BRef<Dictionary>(bEncode.root()).value()) {
      entries[bNode.getKey()] = BRef<String>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, std::string>{{"one", "0123456789"},
                                                          {"two", "asdfghjkl"},
                                                          {"three", "qwerty"}});
  }
}
TEST_CASE("Decode generated exceptions", "[Bencode][Decode][Exceptions]") {
  const Bencode bEncode;
  SECTION("Decode an string without terminating ':' on its length",
          "[Bencode][Decode][Exceptions]") {
    BufferSource bEncodeSource{"26abcdefghijklmnopqrstuvwxyz"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Decode an integer without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource bEncodeSource{"i266"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Decode an list without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource bEncodeSource{"li266ei6780ei88e"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Decode an dictionary without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Decode an string that terminates prematurely",
          "[Bencode][Decode][Exceptions]") {
    BufferSource bEncodeSource{"26:abcdefghijklmno"};
    REQUIRE_THROWS_WITH(
        bEncode.decode(bEncodeSource),
        "ISource Error: Decode buffer empty before decode complete.");
  }
  SECTION("Duplicate dictionary keys",
          "[Bencode][Decode][Dictionary][Exceptions]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567893:two6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Dictionary Keys not in lexical order",
          "[Bencode][Decode][Dictionary][Exceptions]") {
    BufferSource bEncodeSource{
        "d5:three10:01234567893:one6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
}
TEST_CASE("Decode torrent files", "[Bencode][Decode][Torrents]") {
  const Bencode bEncode;
  SECTION("Decode singlefile.torrent", "[Bencode][Decode][Torrents]") {
    FileSource bEncodeSource{prefixTestDataPath(kSingleFileTorrent)};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isDictionary());
  }
  SECTION("Decode singlefile.torrent and check value ",
          "[Bencode][Decode][Torrents]") {
    FileSource bEncodeSource{prefixTestDataPath(kSingleFileTorrent)};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Decode multifile.torrent", "[Bencode][Decode][Torrents]") {
    FileSource bEncodeSource{prefixTestDataPath(kMultiFileTorrent)};
    bEncode.decode(bEncodeSource);
    REQUIRE((bEncode.root()).isDictionary());
  }
  SECTION("Decode multifile.torrent and check value ",
          "[Bencode][Decode][Torrents]") {
    FileSource bEncodeSource{prefixTestDataPath(kMultiFileTorrent)};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent)));
  }
}
TEST_CASE("Decode erroneous torrent files",
          "[Bencode][Decode][Torrents][Error]") {
  const Bencode bEncode;
  SECTION("Decode singlefileerror.torrent",
          "[Bencode][Decode][Torrents][Error]") {
    FileSource bEncodeSource{prefixTestDataPath(kSingleFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
  SECTION("Decode multifileerror.torrent",
          "[Bencode][Decode][Torrents][Error]") {
    FileSource bEncodeSource{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.decode(bEncodeSource), IDecoder::Error);
  }
}