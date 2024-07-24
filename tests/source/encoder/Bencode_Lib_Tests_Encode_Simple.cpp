//
// Unit Tests: Bencode_Lib_Tests_Encode_Simple
//
// Description: Encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode;
  SECTION("Encode an integer (266).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i266e");
  }
  SECTION("Encode an integer (10000).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i10000e");
  }
  SECTION("Encode an integer (-10000).", "[Bencode][Encode][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "i-10000e");
  }
  SECTION("Encode an string ('qwertyuiopas').", "[Bencode][Encode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "12:qwertyuiopas");
  }
  SECTION("Encode an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "26:abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Encode an empty string ('').", "[Bencode][Encode][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "0:");
  }
}
TEST_CASE("Bencode for encode of a table of integer test data",
          "[Bencode][Encode][Integer]") {
  const Bencode bEncode;
  auto [expected] = GENERATE(table<std::string>({"i277e", "i32767e"}));
  BufferSource source(expected);
  BufferDestination destination;
  bEncode.decode(source);
  bEncode.encode(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for encode of a table of string test data",
          "[Bencode][Encode][String]") {
  const Bencode bEncode;
  auto [expected] =
      GENERATE(table<std::string>({"13:qwertyuiopasd", "6:mnbvcx"}));
  BufferSource source(expected);
  BufferDestination destination;
  bEncode.decode(source);
  bEncode.encode(destination);
  REQUIRE(destination.toString() == expected);
}
