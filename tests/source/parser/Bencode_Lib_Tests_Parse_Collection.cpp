#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of collection types (list, dictionary) ",
          "[Bencode][Parse][List]") {
  const Bencode bStringify;
  SECTION("Parse an List", "[Bencode][Parse]") {
    BufferSource source{"li266ei6780ei88e5:threee"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<List>(bStringify.root()));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root()[1]));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root()[2]));
    REQUIRE_FALSE(!isA<String>(bStringify.root()[3]));
    REQUIRE(BRef<Integer>((bStringify.root())[0]).value() == 266);
    REQUIRE(BRef<Integer>((bStringify.root())[1]).value() == 6780);
    REQUIRE(BRef<Integer>((bStringify.root())[2]).value() == 88);
    REQUIRE(BRef<String>((bStringify.root())[3]).value() == "three");
  }
  SECTION("Parse an list of integers.s", "[Bencode][Parse][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    bStringify.parse(source);
    std::vector<int64_t> numbers;
    for (const auto &bNode : BRef<List>(bStringify.root()).value()) {
      numbers.push_back(BRef<Integer>(bNode).value());
    }
    REQUIRE(numbers == std::vector<int64_t>{266, 6780, 88});
  }
  SECTION("Parse an list of strings.s", "[Bencode][Parse][List]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    std::vector<std::string> strings;
    for (const auto &bNode : BRef<List>(bStringify.root()).value()) {
      strings.push_back(BRef<String>(bNode).value());
    }
    REQUIRE(strings == std::vector<std::string>{"sillyy", "poiuytrewqas",
                                                "abcdefghijklmnopqrstuvwxyz"});
  }
  SECTION("Parse an Dictionary", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<Dictionary>(bStringify.root()));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["one"]);
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["two"]);
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["three"]);
    REQUIRE(BRef<Integer>((bStringify.root())["one"]).value() == 1);
    REQUIRE(BRef<Integer>((bStringify.root())["two"]).value() == 2);
    REQUIRE(BRef<Integer>((bStringify.root())["three"]).value() == 3);
  }
  SECTION("Parse an Dictionary of ints.s", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bStringify.parse(source);
    std::map<std::string, int64_t> entries;
    for (const auto &bNode : BRef<Dictionary>(bStringify.root()).value()) {
      entries[bNode.getKey()] = BRef<Integer>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, int64_t>{
                           {"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Parse a Dictionary of strings.s", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    std::map<std::string, std::string> entries;
    for (const auto &bNode : BRef<Dictionary>(bStringify.root()).value()) {
      entries[bNode.getKey()] = BRef<String>(bNode.getBNode()).value();
    }
    REQUIRE(entries == std::map<std::string, std::string>{{"one", "0123456789"},
                                                          {"two", "asdfghjkl"},
                                                          {"three", "qwerty"}});
  }
}