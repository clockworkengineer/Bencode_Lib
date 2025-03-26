#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of simple types (integer, string) ",
          "[Bencode][Parse]") {
  const Bencode bEncode;
  SECTION("Parse an integer", "[Bencode][Parse][Integer]") {
    BufferSource source{"i266e"};
    bEncode.parse(source);
    REQUIRE_FALSE(!isA<Integer>(bEncode.root()));
  }
  SECTION("Parse an integer (266).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i266e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 266);
  }
  SECTION("Parse an integer (1000).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i1000e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 1000);
  }
  SECTION("Parse an integer (0).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i0e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 0);
  }
  SECTION("Parse an negative integer (-666).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i-666e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == -666);
  }
  SECTION("Parse an negative zero (-0) is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bEncode.parse(BufferSource{"i-0e"}), SyntaxError);
  }
  SECTION("Parse an integer with leading zeros is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bEncode.parse(BufferSource{"i0012e"}), SyntaxError);
  }
  SECTION("Parse an empty integer is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bEncode.parse(BufferSource{"ie"}), SyntaxError);
  }
  SECTION("Parse max 64 bit integer (9223372036854775807).",
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i9223372036854775807e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::max());
  }
  SECTION("Parse out of range positive 64 bit integer (9223372036854775808)."
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i9223372036854775808e"};
    REQUIRE_THROWS_AS(bEncode.parse(source), std::out_of_range);
  }
  SECTION("Parse min 64 bit integer (-9223372036854775808).",
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i-9223372036854775808e"};
    bEncode.parse(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::min());
  }
  SECTION("Parse out of range negative 64 bit integer (-9223372036854775809)."
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i-9223372036854775809e"};
    REQUIRE_THROWS_AS(bEncode.parse(source), std::out_of_range);
  }
  SECTION("Parse an string", "[Bencode][Parse][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.parse(source);
    REQUIRE_FALSE(!isA<String>(bEncode.root()));
  }
  SECTION("Parse a string ('qwertyuiopas').", "[Bencode][Parse][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.parse(source);
    REQUIRE(BRef<String>(bEncode.root()).value() == "qwertyuiopas");
  }
  SECTION("Parse a string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Parse][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    bEncode.parse(source);
    REQUIRE(BRef<String>(bEncode.root()).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse a string with zero length", "[Bencode][Parse][String]") {
    REQUIRE_NOTHROW(bEncode.parse(BufferSource{"0:"}));
    REQUIRE(BRef<String>(bEncode.root()).value().empty());
  }
  SECTION("Parse a string with all possible 256 characters.",
          "[Bencode][Parse][String]") {
    std::string str{};
    for (int i = 0; i < 256; i++) {
      str.push_back(static_cast<char>(i));
    }
    bEncode.parse(BufferSource("256:" + str));
    REQUIRE(BRef<String>(bEncode.root()).value() == str);
  }
}