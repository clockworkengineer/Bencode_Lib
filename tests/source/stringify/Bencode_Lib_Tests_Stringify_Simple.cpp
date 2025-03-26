#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify;
  SECTION("Stringify an integer (266).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i266e");
  }
  SECTION("Stringify an integer (10000).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i10000e");
  }
  SECTION("Stringify an integer (-10000).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-10000e");
  }
  SECTION("Stringify an string ('qwertyuiopas').", "[Bencode][Stringify][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "12:qwertyuiopas");
  }
  SECTION("Stringify an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "26:abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Stringify an empty string ('').", "[Bencode][Stringify][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "0:");
  }
}
TEST_CASE("Bencode for stringify of a table of integer test data",
          "[Bencode][Stringify][Integer]") {
  const Bencode bStringify;
  auto [expected] = GENERATE(table<std::string>({"i277e", "i32767e"}));
  BufferSource source(expected);
  BufferDestination destination;
  bStringify.parse(source);
  bStringify.stringify(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for stringify of a table of string test data",
          "[Bencode][Stringify][String]") {
  const Bencode bStringify;
  auto [expected] =
      GENERATE(table<std::string>({"13:qwertyuiopasd", "6:mnbvcx"}));
  BufferSource source(expected);
  BufferDestination destination;
  bStringify.parse(source);
  bStringify.stringify(destination);
  REQUIRE(destination.toString() == expected);
}
