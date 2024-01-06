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
    BufferSource source{"i266e"};
    bEncode.decode(source);
    REQUIRE((bEncode.root()).isInteger());
  }
  SECTION("Decode an integer (266) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i266e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 266);
  }
  SECTION("Decode an integer (1000) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i1000e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 1000);
  }
  SECTION("Decode an integer (0) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i0e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 0);
  }
  SECTION("Decode an negative integer (-666) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i-666e"};
    bEncode.decode(source);
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
    BufferSource source{"i9223372036854775807e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::max());
  }
  SECTION("Decode out of range positive 64 bit integer (9223372036854775808) "
          "and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i9223372036854775808e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), std::out_of_range);
  }
  SECTION("Decode min 64 bit integer (-9223372036854775808) and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i-9223372036854775808e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::min());
  }
  SECTION("Decode out of range negative 64 bit integer (-9223372036854775809) "
          "and check value",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i-9223372036854775809e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), std::out_of_range);
  }
  SECTION("Decode an string", "[Bencode][Decode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.decode(source);
    REQUIRE((bEncode.root()).isString());
  }
  SECTION("Decode a string ('qwertyuiopas') and check value",
          "[Bencode][Decode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.decode(source);
    REQUIRE(BRef<String>(bEncode.root()).value() == "qwertyuiopas");
  }
  SECTION("Decode a string ('abcdefghijklmnopqrstuvwxyz') and check value",
          "[Bencode][Decode][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    bEncode.decode(source);
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
TEST_CASE("Bencode for decode of collection types (list, dictionary) ",
          "[Bencode][Decode][List]") {
  const Bencode bEncode;
  SECTION("Decode an List", "[Bencode][Decode]") {
    BufferSource source{"li266ei6780ei88e5:threee"};
    bEncode.decode(source);
    REQUIRE((bEncode.root()).isList());
    REQUIRE((bEncode.root())[0].isInteger());
    REQUIRE((bEncode.root())[1].isInteger());
    REQUIRE((bEncode.root())[2].isInteger());
    REQUIRE((bEncode.root())[3].isString());
    REQUIRE(BRef<Integer>((bEncode.root())[0]).value() == 266);
    REQUIRE(BRef<Integer>((bEncode.root())[1]).value() == 6780);
    REQUIRE(BRef<Integer>((bEncode.root())[2]).value() == 88);
    REQUIRE(BRef<String>((bEncode.root())[3]).value() == "three");
  }
  SECTION("Decode an list of integers and check values",
          "[Bencode][Decode][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    bEncode.decode(source);
    std::vector<int64_t> numbers;
    for (const auto &bNode : BRef<List>(bEncode.root()).value()) {
      numbers.push_back(BRef<Integer>(bNode).value());
    }
    REQUIRE(numbers == std::vector<int64_t>{266, 6780, 88});
  }
  SECTION("Decode an list of strings and check values",
          "[Bencode][Decode][List]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    std::vector<std::string> strings;
    for (const auto &bNode : BRef<List>(bEncode.root()).value()) {
      strings.push_back(BRef<String>(bNode).value());
    }
    REQUIRE(strings == std::vector<std::string>{"sillyy", "poiuytrewqas",
                                                "abcdefghijklmnopqrstuvwxyz"});
  }
  SECTION("Decode an Dictionary", "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bEncode.decode(source);
    REQUIRE((bEncode.root()).isDictionary());
    REQUIRE((bEncode.root())["one"].isInteger());
    REQUIRE((bEncode.root())["two"].isInteger());
    REQUIRE((bEncode.root())["three"].isInteger());
    REQUIRE(BRef<Integer>((bEncode.root())["one"]).value() == 1);
    REQUIRE(BRef<Integer>((bEncode.root())["two"]).value() == 2);
    REQUIRE(BRef<Integer>((bEncode.root())["three"]).value() == 3);
  }
  SECTION("Decode an Dictionary of ints and check values",
          "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bEncode.decode(source);
    std::map<std::string, int64_t> entries;
    for (const auto &bNode : BRef<Dictionary>(bEncode.root()).value()) {
      entries[bNode.getKey()] = BRef<Integer>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, int64_t>{
                           {"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Decode a Dictionary of strings and check values",
          "[Bencode][Decode][Dictionary]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(source);
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
    BufferSource source{"26abcdefghijklmnopqrstuvwxyz"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
  SECTION("Decode an integer without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"i266"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
  SECTION("Decode an list without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{"li266ei6780ei88e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
  SECTION("Decode an dictionary without a terminating end",
          "[Bencode][Decode][Exceptions]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
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
    BufferSource source{
        "d3:one10:01234567893:two6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
  SECTION("Dictionary Keys not in lexical order",
          "[Bencode][Decode][Dictionary][Exceptions]") {
    BufferSource source{
        "d5:three10:01234567893:one6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
}
TEST_CASE("Decode torrent files", "[Bencode][Decode][Torrents]") {
  const Bencode bEncode;
  SECTION("Decode singlefile.torrent", "[Bencode][Decode][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    bEncode.decode(source);
    REQUIRE((bEncode.root()).isDictionary());
  }
  SECTION("Decode singlefile.torrent and check value ",
          "[Bencode][Decode][Torrents]") {
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
    REQUIRE((bEncode.root()).isDictionary());
  }
  SECTION("Decode multifile.torrent and check value ",
          "[Bencode][Decode][Torrents]") {
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
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
  SECTION("Decode multifileerror.torrent",
          "[Bencode][Decode][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncode.decode(source), IDecoder::Error);
  }
}