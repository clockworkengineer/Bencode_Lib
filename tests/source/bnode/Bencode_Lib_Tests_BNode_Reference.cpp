//
// Unit Tests: Bencode_Lib_Tests_BNode_Reference
//
// Description: BNode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check BNode reference functions work.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"10:0123456789"};
    bEncode.decode(source);
    REQUIRE(BRef<String>((bEncode.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    REQUIRE(BRef<List>((bEncode.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(source);
    REQUIRE(BRef<Dictionary>((bEncode.root())).size() == 4);
  }
  SECTION("BRef check node type correct.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bEncode.decode(source);
    REQUIRE_THROWS_AS(BRef<Dictionary>(bEncode.root()), BNode::Error);
    REQUIRE_THROWS_WITH(BRef<Dictionary>(bEncode.root()),
                        "BNode Error: Node not a dictionary.");
  }
}