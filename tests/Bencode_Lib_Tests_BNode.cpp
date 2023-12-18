//
// Unit Tests: Bencode
//
// Description: BNode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]") {
  const Bencode bEncode;
  SECTION("Decode dictionary and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())["three"]).value() == "qwerty");
  }
  SECTION("Decode list and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]).value() == "poiuytrewqas");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode list with embedded dictionary and check its components using "
          "indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{"l6:sillyyd3:one10:01234567895:three6:qwerty3:"
                               "two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())[1]["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())[1]["three"]).value() == "qwerty");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode dictionary and check an invalid key generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())["onee"].isDictionary(), BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())["onee"].isDictionary(),
                        "BNode Error: Invalid key used in dictionary.");
  }
  SECTION("Decode list and check an invalid index generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())[3].isList(), BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())[3].isList(),
                        "BNode Error: Invalid index used in list.");
  }
}
TEST_CASE("Check BNode reference functions work.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"i45500e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"10:0123456789"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<List>((bEncode.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Dictionary>((bEncode.root())).size() == 4);
  }
  SECTION("BRef check node type correct.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"i45500e"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS(BRef<Dictionary>(bEncode.root()), BNode::Error);
    REQUIRE_THROWS_WITH(BRef<Dictionary>(bEncode.root()),
                        "BNode Error: Node not a dictionary.");
  }
}