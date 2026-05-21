#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Parse string length prefix with plus sign", "[Bencode][Parse][String][Exception]") {
  const Bencode b;
  REQUIRE_THROWS_AS(b.parse(BufferSource{"+1:a"}), SyntaxError);
}

TEST_CASE("Parse string length prefix with invalid characters", "[Bencode][Parse][String][Exception]") {
  const Bencode b;
  REQUIRE_THROWS_AS(b.parse(BufferSource{"1a:abc"}), SyntaxError);
}

TEST_CASE("Parse integer with explicit plus sign", "[Bencode][Parse][Integer][Exception]") {
  const Bencode b;
  REQUIRE_THROWS_AS(b.parse(BufferSource{"i+1e"}), SyntaxError);
}

TEST_CASE("Parse dictionary with key length mismatch", "[Bencode][Parse][Dictionary][Exception]") {
  const Bencode b;
  REQUIRE_THROWS_AS(b.parse(BufferSource{"d2:ai1ee"}), SyntaxError);
}

TEST_CASE("Parse list containing invalid element type", "[Bencode][Parse][List][Exception]") {
  const Bencode b;
  REQUIRE_THROWS_AS(b.parse(BufferSource{"lXe"}), SyntaxError);
}
