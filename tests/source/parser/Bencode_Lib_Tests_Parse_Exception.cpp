#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Parse generated Exception", "[Bencode][Parse][Exception]") {
  const Bencode bStringify;
  SECTION("Parse an string without terminating ':' on its length",
          "[Bencode][Parse][Exception]") {
    BufferSource source{"26abcdefghijklmnopqrstuvwxyz"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse an integer without a terminating end",
          "[Bencode][Parse][Exception]") {
    BufferSource source{"i266"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse an list without a terminating end",
          "[Bencode][Parse][Exception]") {
    BufferSource source{"li266ei6780ei88e"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse an dictionary without a terminating end",
          "[Bencode][Parse][Exception]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse an string that terminates prematurely",
          "[Bencode][Parse][Exception]") {
    BufferSource source{"26:abcdefghijklmno"};
    REQUIRE_THROWS_WITH(
        bStringify.parse(source),
        "ISource Error: Parse buffer empty before parse complete.");
  }
  SECTION("Duplicate dictionary keys",
          "[Bencode][Parse][Dictionary][Exception]") {
    BufferSource source{"d3:one10:01234567893:two6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Dictionary Keys not in lexical order",
          "[Bencode][Parse][Dictionary][Exception]") {
    BufferSource source{"d5:three10:01234567893:one6:qwerty3:two9:asdfghjkle"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
  SECTION("Parse an Dictionary of ints that terminates early",
          "[Bencode][Parse][Dictionary][Exception]") {
    BufferSource source{"d3:onei1e5:threei3ee3:twoi2ee"};
    REQUIRE_THROWS_AS(bStringify.parse(source), SyntaxError);
  }
    SECTION("Parse a string with no length", "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{":"}), SyntaxError);
  }
  SECTION("Parse a string with negative length", "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"-2:ww"}), SyntaxError);
  }
  SECTION("Parse a string with max length (buffer overflow attempt)",
          "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"9223372036854775807:ww"}),
                      std::runtime_error);
  }
}