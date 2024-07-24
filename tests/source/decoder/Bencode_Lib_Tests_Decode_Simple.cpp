#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for decode of simple types (integer, string) ",
          "[Bencode][Decode]") {
  const Bencode bEncode;
  SECTION("Decode an integer", "[Bencode][Decode][Integer]") {
    BufferSource source{"i266e"};
    bEncode.decode(source);
    REQUIRE_FALSE(!isA<Integer>(bEncode.root()));
  }
  SECTION("Decode an integer (266).", "[Bencode][Decode][Integer]") {
    BufferSource source{"i266e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 266);
  }
  SECTION("Decode an integer (1000).", "[Bencode][Decode][Integer]") {
    BufferSource source{"i1000e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 1000);
  }
  SECTION("Decode an integer (0).", "[Bencode][Decode][Integer]") {
    BufferSource source{"i0e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == 0);
  }
  SECTION("Decode an negative integer (-666).", "[Bencode][Decode][Integer]") {
    BufferSource source{"i-666e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() == -666);
  }
  SECTION("Decode an negative zero (-0) is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"i-0e"}), SyntaxError);
  }
  SECTION("Decode an integer with leading zeros is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"i0012e"}), SyntaxError);
  }
  SECTION("Decode an empty integer is a syntax error",
          "[Bencode][Decode][Integer]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"ie"}), SyntaxError);
  }
  SECTION("Decode max 64 bit integer (9223372036854775807).",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i9223372036854775807e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::max());
  }
  SECTION("Decode out of range positive 64 bit integer (9223372036854775808)."
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i9223372036854775808e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), std::out_of_range);
  }
  SECTION("Decode min 64 bit integer (-9223372036854775808).",
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i-9223372036854775808e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>(bEncode.root()).value() ==
            std::numeric_limits<int64_t>::min());
  }
  SECTION("Decode out of range negative 64 bit integer (-9223372036854775809)."
          "[Bencode][Decode][Integer]") {
    BufferSource source{"i-9223372036854775809e"};
    REQUIRE_THROWS_AS(bEncode.decode(source), std::out_of_range);
  }
  SECTION("Decode an string", "[Bencode][Decode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.decode(source);
    REQUIRE_FALSE(!isA<String>(bEncode.root()));
  }
  SECTION("Decode a string ('qwertyuiopas').", "[Bencode][Decode][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bEncode.decode(source);
    REQUIRE(BRef<String>(bEncode.root()).value() == "qwertyuiopas");
  }
  SECTION("Decode a string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Decode][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    bEncode.decode(source);
    REQUIRE(BRef<String>(bEncode.root()).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode a string with zero length", "[Bencode][Decode][String]") {
    REQUIRE_NOTHROW(bEncode.decode(BufferSource{"0:"}));
    REQUIRE(BRef<String>(bEncode.root()).value().empty());
  }
  SECTION("Decode a string with no length", "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{":"}), SyntaxError);
  }
  SECTION("Decode a string with negative length", "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"-2:ww"}), SyntaxError);
  }
  SECTION("Decode a string with max length (buffer overflow attempt)",
          "[Bencode][Decode][String]") {
    REQUIRE_THROWS_AS(bEncode.decode(BufferSource{"9223372036854775807:ww"}),
                      std::runtime_error);
  }
}