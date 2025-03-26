#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]") {
  const Bencode bStringify;
  SECTION("Parse dictionary and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE(BRef<String>((bStringify.root())["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bStringify.root())["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bStringify.root())["three"]).value() == "qwerty");
  }
  SECTION("Parse list and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(BRef<String>((bStringify.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bStringify.root())[1]).value() == "poiuytrewqas");
    REQUIRE(BRef<String>((bStringify.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse list with embedded dictionary and check its components using "
          "indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{"l6:sillyyd3:one10:01234567895:three6:qwerty3:"
                               "two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(BRef<String>((bStringify.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bStringify.root())[1]["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bStringify.root())[1]["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bStringify.root())[1]["three"]).value() == "qwerty");
    REQUIRE(BRef<String>((bStringify.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse dictionary and check an invalid key generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(isA<Dictionary>(bStringify.root()["onee"]), Dictionary::Error);
    REQUIRE_THROWS_WITH(isA<Dictionary>(bStringify.root()["onee"]),
                        "Dictionary Error: Invalid key used in dictionary.");
  }
  SECTION("Parse list and check an invalid index generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(isA<List>(bStringify.root()[3]), List::Error);
    REQUIRE_THROWS_WITH(isA<List>(bStringify.root()[3]),
                        "List Error: Invalid index used in list.");
  }
}