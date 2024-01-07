//
// Unit Tests: Bencode
//
// Description: XML encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("XML encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode(std::make_unique<XML_Encoder>().release());
  SECTION("XML encode an integer (266) and check value",
          "[Bencode][Encode][XML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>266</root>)");
  }
  SECTION("XML encode an integer (10000) and check value",
          "[Bencode][Encode][XML][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>10000</root>)");
  }
  SECTION("XML encode an string ('qwertyuiopas') and check its value",
          "[Bencode][Encode][XML][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>"qwertyuiopas"</root>)");
  }
  SECTION("XML encode an string ('abcdefghijklmnopqrstuvwxyz') and check its "
          "value ",
          "[Bencode][Encode][XML][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>"abcdefghijklmnopqrstuvwxyz"</root>)");
  }
}
TEST_CASE("XML encode of collection types (list, dictionary) ",
          "[Bencode][Encode][XML]") {
  const Bencode bEncode(std::make_unique<XML_Encoder>().release());
  SECTION("XML encode an List of integers('li266ei6780ei88ee') and check value",
          "[Bencode][Encode][XML][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Array0>266</Array0><Array1>6780</Array1><Array2>88</Array2></root>)");
  }
  SECTION("XML encode an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee') and check value",
          "[Bencode][Encode][XML][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Array0>266</Array0><Array1>"five"</Array1><Array2>6780</Array2><Array3>"one"</Array3><Array4>88</Array4></root>)");
  }
  SECTION("XML encode an Dictionary of integers and check value",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>1</one><three>3</three><two>2</two></root>)");
  }
  SECTION("XML encode an Dictionary of strings and check value",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>"0123456789"</one><three>"qwerty"</three><two>"asdfghjkl"</two></root>)");
  }
  SECTION("XML encode an Dictionary of arrays and check value",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one><Array0>1</Array0><Array1>2</Array1><Array2>3</Array2></one><two><Array0>5555</Array0><Array1>"four"</Array1></two></root>)");
  }
}