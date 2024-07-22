//
// Unit Tests: Bencode_Lib_Tests_JSON_Encode
//
// Description: JSON encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"
#include "JSON_Encoder.hpp"

TEST_CASE("JSON encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode(makeEncoder<JSON_Encoder>());
  SECTION("JSON encode an integer (266).", "[Bencode][Encode][JSON][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "266");
  }
  SECTION("JSON encode an integer (10000).",
          "[Bencode][Encode][JSON][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "10000");
  }
  SECTION("JSON encode an integer (-10000).",
          "[Bencode][Encode][JSON][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "-10000");
  }
  SECTION("JSON encode an string ('qwertyuiopas').",
          "[Bencode][Encode][JSON][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"("qwertyuiopas")");
  }
  SECTION("JSON encode an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][JSON][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"("abcdefghijklmnopqrstuvwxyz")");
  }
  SECTION("JSON encode an empty string.", "[Bencode][Encode][JSON][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"("")");
  }
  SECTION("JSON encode an string with unprintable character. "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][JSON][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += static_cast<char>(1);
    escaped += static_cast<char>(2);
    BufferSource source{escaped};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"("abcdefghijklmnopqrstuvwxyz\u0000\u0001\u0002")");
  }
}
TEST_CASE("JSON encode of collection types (list, dictionary) ",
          "[Bencode][Encode][JSON]") {
  const Bencode bEncode(makeEncoder<JSON_Encoder>());
  SECTION("JSON encode an List of integers('li266ei6780ei88ee').",
          "[Bencode][Encode][JSON][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"([266,6780,88])");
  }
  SECTION("JSON encode an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Encode][JSON][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"([266,"five",6780,"one",88])");
  }
  SECTION("JSON encode an empty List).", "[Bencode][Encode][JSON][List]") {
    BufferSource source{"le"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"([])");
  }
  SECTION("JSON encode an Dictionary of integers.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"({"one" : 1,"three" : 3,"two" : 2})");
  }
  SECTION("JSON encode an Dictionary of strings.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"({"one" : "0123456789","three" : "qwerty","two" : "asdfghjkl"})");
  }
  SECTION("JSON encode an Dictionary of arrays.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"({"one" : [1,2,3],"two" : [5555,"four"]})");
  }
  SECTION("JSON encode an empty Dictionary.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"de"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"({})");
  }
  SECTION("JSON encode an empty List of empty Dictionaries.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"ldededededee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"([{},{},{},{},{}])");
  }
  SECTION("JSON encode an empty List of empty lists.",
          "[Bencode][Encode][JSON][Dictionary]") {
    BufferSource source{"llelelelelee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == R"([[],[],[],[],[]])");
  }
}