//
// Unit Tests: Bencode
//
// Description: Encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("Bencode for JSON encode of simple types (integer, string) ",
          "[Bencode][Encode]") {

  const Bencode bEncode(std::make_unique<JSON_Encoder>().release());
  SECTION("JSON encode an integer (266) and check value",
          "[Bencode][Encode][JSON][Integer]") {
    BufferSource bEncodeSource{"i266e"};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "266");
  }
  SECTION("JSON encode an integer (10000) and check value",
          "[Bencode][Encode][JSON][Integer]") {
    BufferSource bEncodeSource{"i10000e"};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "10000");
  }
  SECTION("JSON encode an string ('qwertyuiopas') and check its value",
          "[Bencode][Encode][JSON][String]") {
    BufferSource bEncodeSource{"12:qwertyuiopas"};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            R"("qwertyuiopas")");
  }
  SECTION("JSON encode an string ('abcdefghijklmnopqrstuvwxyz') and check its "
          "value ",
          "[Bencode][Encode][JSON][String]") {
    BufferSource bEncodeSource{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            R"("abcdefghijklmnopqrstuvwxyz")");
  }
}
// TEST_CASE("JSON encode of a table of integer test data",
//           "[Bencode][Encode][JSON][Integer]") {
//   const Bencode bEncode(std::make_unique<JSON_Encoder>().release());
//   auto [expected] = GENERATE(table<std::string>({"i277e", "i32767e"}));
//   BufferSource bEncodeSource(expected);
//   BufferDestination bEncodeDestination;
//   bEncode.decode(bEncodeSource);
//   bEncode.encode(bEncodeDestination);
//   REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
// }

// TEST_CASE("Bencode for encode of a table of string test data",
//           "[Bencode][Encode][String]") {
//   const Bencode bEncode;
//   auto [expected] =
//       GENERATE(table<std::string>({"13:qwertyuiopasd", "6:mnbvcx"}));
//   BufferSource bEncodeSource(expected);
//   BufferDestination bEncodeDestination;
//   bEncode.decode(bEncodeSource);
//   bEncode.encode(bEncodeDestination);
//   REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
// }
TEST_CASE("JSON encode of collection types (list, dictionary) ",
          "[Bencode][Encode][JSON]") {
  const Bencode bEncode(std::make_unique<JSON_Encoder>().release());
  SECTION(
      "JSON encode an List of integers('li266ei6780ei88ee') and check value",
      "[Bencode][Encode][JSON][List]") {
    std::string expected{"li266ei6780ei88ee"};
    BufferSource bEncodeSource(expected);
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            R"([266,6780,88])");
  }

  SECTION("JSON encode an Dictionary of integers and check value",
          "[Bencode][Encode][JSON][Dictionary]") {
    std::string expected{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferSource bEncodeSource(expected);
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) ==
            R"({"one" : 1,"three" : 3,"two" : 2})");
  }
  SECTION("JSON encode  an Dictionary of strings and check value",
          "[Bencode][Encode][JSON][Dictionary]") {
    std::string expected{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferSource bEncodeSource(expected);
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(
        bufferToString(bEncodeDestination.getBuffer()) ==
        R"({"one" : "0123456789","three" : "qwerty","two" : "asdfghjkl"})");
  }
}