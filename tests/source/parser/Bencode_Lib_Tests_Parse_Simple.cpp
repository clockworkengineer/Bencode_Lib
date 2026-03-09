#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of simple types (integer, string) ",
          "[Bencode][Parse]") {
  const Bencode bStringify;
  SECTION("Parse an integer", "[Bencode][Parse][Integer]") {
    BufferSource source{"i266e"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<Integer>(bStringify.root()));
  }
  SECTION("Parse an integer (266).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i266e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() == 266);
  }
  SECTION("Parse an integer (1000).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i1000e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() == 1000);
  }
  SECTION("Parse an integer (0).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i0e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() == 0);
  }
  SECTION("Parse an negative integer (-666).", "[Bencode][Parse][Integer]") {
    BufferSource source{"i-666e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() == -666);
  }
  SECTION("Parse an negative zero (-0) is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"i-0e"}), SyntaxError);
  }
  SECTION("Parse an integer with leading zeros is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"i0012e"}), SyntaxError);
  }
  SECTION("Parse an empty integer is a syntax error",
          "[Bencode][Parse][Integer]") {
    REQUIRE_THROWS_AS(bStringify.parse(BufferSource{"ie"}), SyntaxError);
  }
  SECTION("Parse max 64 bit integer (9223372036854775807).",
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i9223372036854775807e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() ==
            std::numeric_limits<Bencode::IntegerType>::max());
  }
  SECTION("Parse out of range positive 64 bit integer (9223372036854775808)."
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i9223372036854775808e"};
    REQUIRE_THROWS_AS(bStringify.parse(source), std::out_of_range);
  }
  SECTION("Parse min 64 bit integer (-9223372036854775808).",
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i-9223372036854775808e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>(bStringify.root()).value() ==
            std::numeric_limits<Bencode::IntegerType>::min());
  }
  SECTION("Parse out of range negative 64 bit integer (-9223372036854775809)."
          "[Bencode][Parse][Integer]") {
    BufferSource source{"i-9223372036854775809e"};
    REQUIRE_THROWS_AS(bStringify.parse(source), std::out_of_range);
  }
  SECTION("Parse an string", "[Bencode][Parse][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<String>(bStringify.root()));
  }
  SECTION("Parse a string ('qwertyuiopas').", "[Bencode][Parse][String]") {
    BufferSource source{"12:qwertyuiopas"};
    bStringify.parse(source);
    REQUIRE(NRef<String>(bStringify.root()).value() == "qwertyuiopas");
  }
  SECTION("Parse a string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Parse][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    bStringify.parse(source);
    REQUIRE(NRef<String>(bStringify.root()).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse a string with zero length", "[Bencode][Parse][String]") {
    REQUIRE_NOTHROW(bStringify.parse(BufferSource{"0:"}));
    REQUIRE(NRef<String>(bStringify.root()).value().empty());
  }
  SECTION("Parse a string with all possible 256 characters.",
          "[Bencode][Parse][String]") {
    std::string str{};
    for (int i = 0; i < 256; i++) {
      str.push_back(static_cast<char>(i));
    }
    bStringify.parse(BufferSource("256:" + str));
    REQUIRE(NRef<String>(bStringify.root()).value() == str);
  }
  SECTION("Parse an integer (32767).", "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i32767e"});
    REQUIRE(NRef<Integer>(bStringify.root()).value() == 32767);
  }
  SECTION("Parse a large negative integer (-1000000).",
          "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i-1000000e"});
    REQUIRE(NRef<Integer>(bStringify.root()).value() == -1000000);
  }
  SECTION("Parse integer one (1).", "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i1e"});
    REQUIRE(NRef<Integer>(bStringify.root()).value() == 1);
  }
  SECTION("Parse integer minus one (-1).", "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i-1e"});
    REQUIRE(NRef<Integer>(bStringify.root()).value() == -1);
  }
  SECTION("Parsed integer node is not a string.", "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i42e"});
    REQUIRE_FALSE(isA<String>(bStringify.root()));
  }
  SECTION("Parsed integer node is not a list.", "[Bencode][Parse][Integer]") {
    bStringify.parse(BufferSource{"i42e"});
    REQUIRE_FALSE(isA<List>(bStringify.root()));
  }
  SECTION("Parse a single-character string.", "[Bencode][Parse][String]") {
    bStringify.parse(BufferSource{"1:z"});
    REQUIRE(NRef<String>(bStringify.root()).value() == "z");
  }
  SECTION("Parse a string with spaces.", "[Bencode][Parse][String]") {
    bStringify.parse(BufferSource{"11:hello world"});
    REQUIRE(NRef<String>(bStringify.root()).value() == "hello world");
  }
  SECTION("Parse a string with numeric characters.",
          "[Bencode][Parse][String]") {
    bStringify.parse(BufferSource{"10:1234567890"});
    REQUIRE(NRef<String>(bStringify.root()).value() == "1234567890");
  }
  SECTION("Parsed string node is not an integer.", "[Bencode][Parse][String]") {
    bStringify.parse(BufferSource{"5:hello"});
    REQUIRE_FALSE(isA<Integer>(bStringify.root()));
  }
  SECTION("Parsed string node is not a dictionary.",
          "[Bencode][Parse][String]") {
    bStringify.parse(BufferSource{"5:hello"});
    REQUIRE_FALSE(isA<Dictionary>(bStringify.root()));
  }
}