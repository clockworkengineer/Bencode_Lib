#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for stringify of collection types (list, dictionary) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify;
  SECTION("Stringify an List of integers('li266ei6780ei88ee').",
          "[Bencode][Stringify][List]") {
    std::string expected{"li266ei6780ei88ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an List of strings "
          "('l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze')."
          "[Bencode][Stringify][List]") {
    std::string expected{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify ampty List."
          "[Bencode][Stringify][List]") {
    std::string expected{"le"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an Dictionary of integers.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an Dictionary of strings.", "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an empty  Dictionary.", "[Bencode][Stringify][Dictionary]") {
    std::string expected{"de"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
}