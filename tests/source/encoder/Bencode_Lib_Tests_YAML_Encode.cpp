#include "Bencode_Lib_Tests.hpp"
#include "YAML_Encoder.hpp"

TEST_CASE("YAML encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode(makeEncoder<YAML_Encoder>());
  SECTION("YAML encode an integer (266).", "[Bencode][Encode][YAML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n266\n...\n");
  }
  SECTION("YAML encode an integer (10000).",
          "[Bencode][Encode][YAML][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n10000\n...\n");
  }
  SECTION("YAML encode an integer (-10000).",
          "[Bencode][Encode][YAML][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n-10000\n...\n");
  }
  SECTION("YAML encode an string ('qwertyuiopas').",
          "[Bencode][Encode][YAML][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n\"qwertyuiopas\"\n...\n");
  }
  SECTION("YAML encode an string ('abcdefghijklmnopqrstuvwxyz')."
          "[Bencode][Encode][YAML][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n\"abcdefghijklmnopqrstuvwxyz\"\n...\n");
  }
  SECTION("YAML encode an empty string."
          "[Bencode][Encode][YAML][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n\"\"\n...\n");
  }
  SECTION("YAML encode an string with unprintable characters "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][YAML][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += 1;
    escaped += 2;
    BufferSource source{escaped};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            "---\n\"abcdefghijklmnopqrstuvwxyz\\u0000\\u0001\\u0002\"\n...\n");
  }
}
TEST_CASE("YAML encode of collection types (list, dictionary) ",
          "[Bencode][Encode][YAML]") {
  const Bencode bEncode(makeEncoder<YAML_Encoder>());
  SECTION("YAML encode an List of integers('li266ei6780ei88ee').",
          "[Bencode][Encode][YAML][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n- 266\n- 6780\n- 88\n...\n");
  }
  SECTION("YAML encode an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Encode][YAML][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            "---\n- 266\n- \"five\"\n- 6780\n- \"one\"\n- 88\n...\n");
  }
  SECTION("YAML encode an empty List.", "[Bencode][Encode][YAML][List]") {
    BufferSource source{"le"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n[]\n...\n");
  }
  SECTION("YAML encode an Dictionary of integers.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n\"one\": 1\n\"three\": 3\n\"two\": 2\n...\n");
  }
  SECTION("YAML encode an Dictionary of strings.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            "---\n\"one\": \"0123456789\"\n\"three\": \"qwerty\"\n\"two\": \"asdfghjkl\"\n...\n");
  }
  SECTION("YAML encode an Dictionary of arrays.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        "---\n\"one\": \n  - 1\n  - 2\n  - 3\n\"two\": \n  - 5555\n  - \"four\"\n...\n");
  }
  SECTION("YAML encode an empty Dictionary.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"de"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n{}\n...\n");
  }
  SECTION("YAML encode an empty List of empty Dictionaries.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"ldededededee"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            "---\n- {}\n- {}\n- {}\n- {}\n- {}\n...\n");
  }
  SECTION("YAML encode an list of empty lists.",
          "[Bencode][Encode][YAML][Dictionary]") {
    BufferSource source{"llelelelelee"};
    BufferDestination destination;
    bEncode.parse(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() == "---\n- []\n- []\n- []\n- []\n- []\n...\n");
  }
}
