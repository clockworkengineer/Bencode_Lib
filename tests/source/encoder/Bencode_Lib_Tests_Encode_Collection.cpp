#include "Bencode_Lib_Tests.hpp"

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