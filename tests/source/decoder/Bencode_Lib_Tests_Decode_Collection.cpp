//
// Unit Tests: Bencode_Lib_Tests_Decode_Collection
//
// Description: Decode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for decode of collection types (list, dictionary) ",
          "[Bencode][Decode][List]") {
  const Bencode bEncode;
  SECTION("Decode an List", "[Bencode][Decode]") {
    BufferSource source{"li266ei6780ei88e5:threee"};
    bEncode.decode(source);
    REQUIRE_FALSE(!isA<List>(bEncode.root()));
    REQUIRE_FALSE(!isA<Integer>(bEncode.root()[1]));
    REQUIRE_FALSE(!isA<Integer>(bEncode.root()[2]));
    REQUIRE_FALSE(!isA<String>(bEncode.root()[3]));
    REQUIRE(BRef<Integer>((bEncode.root())[0]).value() == 266);
    REQUIRE(BRef<Integer>((bEncode.root())[1]).value() == 6780);
    REQUIRE(BRef<Integer>((bEncode.root())[2]).value() == 88);
    REQUIRE(BRef<String>((bEncode.root())[3]).value() == "three");
  }
  SECTION("Decode an list of integers.s", "[Bencode][Decode][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    bEncode.decode(source);
    std::vector<int64_t> numbers;
    for (const auto &bNode : BRef<List>(bEncode.root()).value()) {
      numbers.push_back(BRef<Integer>(bNode).value());
    }
    REQUIRE(numbers == std::vector<int64_t>{266, 6780, 88});
  }
  SECTION("Decode an list of strings.s", "[Bencode][Decode][List]") {
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
    REQUIRE_FALSE(!isA<Dictionary>(bEncode.root()));
    REQUIRE_FALSE(!isA<Integer>(bEncode.root())["one"]);
    REQUIRE_FALSE(!isA<Integer>(bEncode.root())["two"]);
    REQUIRE_FALSE(!isA<Integer>(bEncode.root())["three"]);
    REQUIRE(BRef<Integer>((bEncode.root())["one"]).value() == 1);
    REQUIRE(BRef<Integer>((bEncode.root())["two"]).value() == 2);
    REQUIRE(BRef<Integer>((bEncode.root())["three"]).value() == 3);
  }
  SECTION("Decode an Dictionary of ints.s", "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bEncode.decode(source);
    std::map<std::string, int64_t> entries;
    for (const auto &bNode : BRef<Dictionary>(bEncode.root()).value()) {
      entries[bNode.getKey()] = BRef<Integer>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, int64_t>{
                           {"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Decode a Dictionary of strings.s", "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
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