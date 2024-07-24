#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Decode generated Exception", "[Bencode][Decode][Exception]") {
  const Bencode bEncode;
  SECTION("Decode an string without terminating ':' on its length",
          "[Bencode][Decode][Exception]") {
    BufferSource source{"26abcdefghijklmnopqrstuvwxyz"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an integer without a terminating end",
          "[Bencode][Decode][Exception]") {
    BufferSource source{"i266"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an list without a terminating end",
          "[Bencode][Decode][Exception]") {
    BufferSource source{"li266ei6780ei88e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an dictionary without a terminating end",
          "[Bencode][Decode][Exception]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an string that terminates prematurely",
          "[Bencode][Decode][Exception]") {
    BufferSource source{"26:abcdefghijklmno"};
    REQUIRE_THROWS_WITH(
        bEncode.decode(source),
        "ISource Error: Decode buffer empty before decode complete.");
  }
  SECTION("Duplicate dictionary keys",
          "[Bencode][Decode][Dictionary][Exception]") {
    BufferSource source{"d3:one10:01234567893:two6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Dictionary Keys not in lexical order",
          "[Bencode][Decode][Dictionary][Exception]") {
    BufferSource source{"d5:three10:01234567893:one6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
  SECTION("Decode an Dictionary of ints that terminates early",
          "[Bencode][Decode][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3ee3:twoi2ee"};
    REQUIRE_THROWS_AS(bEncode.decode(source), SyntaxError);
  }
}