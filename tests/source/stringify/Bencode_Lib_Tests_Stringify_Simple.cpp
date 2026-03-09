#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify;
  SECTION("Stringify an integer (266).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i266e");
  }
  SECTION("Stringify an integer (10000).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i10000e");
  }
  SECTION("Stringify an integer (-10000).", "[Bencode][Stringify][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-10000e");
  }
  SECTION("Stringify an string ('qwertyuiopas').",
          "[Bencode][Stringify][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "12:qwertyuiopas");
  }
  SECTION("Stringify an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "26:abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Stringify an empty string ('').", "[Bencode][Stringify][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "0:");
  }
}
TEST_CASE("Bencode for stringify of a table of integer test data",
          "[Bencode][Stringify][Integer]") {
  const Bencode bStringify;
  auto [expected] = GENERATE(table<std::string>({"i277e", "i32767e"}));
  BufferSource source(expected);
  BufferDestination destination;
  bStringify.parse(source);
  bStringify.stringify(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for stringify of a table of string test data",
          "[Bencode][Stringify][String]") {
  const Bencode bStringify;
  auto [expected] =
      GENERATE(table<std::string>({"13:qwertyuiopasd", "6:mnbvcx"}));
  BufferSource source(expected);
  BufferDestination destination;
  bStringify.parse(source);
  bStringify.stringify(destination);
  REQUIRE(destination.toString() == expected);
}
TEST_CASE("Bencode for stringify of simple types (integer, string) extra",
          "[Bencode][Stringify]") {
  const Bencode bStringify;
  SECTION("Stringify integer zero.", "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i0e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i0e");
  }
  SECTION("Stringify integer one.", "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i1e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i1e");
  }
  SECTION("Stringify integer minus one.", "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-1e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-1e");
  }
  SECTION("Stringify max 64-bit integer.", "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i9223372036854775807e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i9223372036854775807e");
  }
  SECTION("Stringify min 64-bit integer.", "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-9223372036854775808e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-9223372036854775808e");
  }
  SECTION("Stringify a single-character string.",
          "[Bencode][Stringify][String]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"1:z"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "1:z");
  }
  SECTION("Stringify a string with spaces.", "[Bencode][Stringify][String]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"11:hello world"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "11:hello world");
  }
  SECTION("Stringify a string with numeric characters.",
          "[Bencode][Stringify][String]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"10:1234567890"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "10:1234567890");
  }
  SECTION("Stringify a string with all 256 byte values.",
          "[Bencode][Stringify][String]") {
    std::string payload{};
    for (int i = 0; i < 256; i++) {
      payload.push_back(static_cast<char>(i));
    }
    const std::string encoded = "256:" + payload;
    BufferDestination destination;
    bStringify.parse(BufferSource{encoded});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == encoded);
  }
  SECTION("Stringify a large negative integer.",
          "[Bencode][Stringify][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-1000000e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-1000000e");
  }
}
