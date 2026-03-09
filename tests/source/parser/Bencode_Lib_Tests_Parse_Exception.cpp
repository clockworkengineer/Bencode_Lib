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
  SECTION("Parse a string with no length",
          "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{":"}), SyntaxError);
  }
  SECTION("Parse a string with negative length",
          "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"-2:ww"}), SyntaxError);
  }
  SECTION("Parse a string with max length (buffer overflow attempt)",
          "[Bencode][Parse][String][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"9223372036854775807:ww"}),
                      std::runtime_error);
  }
  SECTION("Parse integer with no digits before terminator.",
          "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"ie"}), SyntaxError);
  }
  SECTION("Parse integer with leading zero.", "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"i01e"}), SyntaxError);
  }
  SECTION("Parse negative zero integer.", "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"i-0e"}), SyntaxError);
  }
  SECTION("Parse list with unterminated nested list.",
          "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"lli1ei2ee"}), SyntaxError);
  }
  SECTION("Parse dictionary with value missing.",
          "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"d3:onee"}), SyntaxError);
  }
  SECTION("Parse completely empty input.", "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{""}), std::runtime_error);
  }
  SECTION("Parse an unknown type tag.", "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"x99e"}), SyntaxError);
  }
  SECTION("Parse integer missing 'e' terminator with trailing data.",
          "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"i42li1ee"}), SyntaxError);
  }
  SECTION("Parse string with REQUIRE_THROWS_WITH on premature end.",
          "[Bencode][Parse][Exception]") {
    REQUIRE_THROWS_WITH(
        bStringify.parse(BufferSource{"10:short"}),
        "ISource Error: Parse buffer empty before parse complete.");
  }
  SECTION("Duplicate dictionary keys generates correct message.",
          "[Bencode][Parse][Dictionary][Exception]") {
    REQUIRE_THROWS_WITH(
        bStringify.parse(BufferSource{"d3:key5:first3:key6:seconde"}),
        "Bencode Syntax Error: Duplicate dictionary key.");
  }
  SECTION("Dictionary keys out of order generates correct message.",
          "[Bencode][Parse][Dictionary][Exception]") {
    REQUIRE_THROWS_WITH(
        bStringify.parse(BufferSource{"d1:bi2e1:ai1ee"}),
        "Bencode Syntax Error: Dictionary keys not in sequence.");
  }
}