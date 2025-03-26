#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check BNode reference functions work.",
          "[Bencode][BNode][Reference]") {
  const Bencode bStringify;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bStringify.parse(source);
    REQUIRE(BRef<Integer>((bStringify.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"10:0123456789"};
    bStringify.parse(source);
    REQUIRE(BRef<String>((bStringify.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(BRef<List>((bStringify.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE(BRef<Dictionary>((bStringify.root())).size() == 4);
  }
  SECTION("BRef check node type correct.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(BRef<Dictionary>(bStringify.root()), BNode::Error);
    REQUIRE_THROWS_WITH(BRef<Dictionary>(bStringify.root()),
                        "BNode Error: Node not a dictionary.");
  }
}