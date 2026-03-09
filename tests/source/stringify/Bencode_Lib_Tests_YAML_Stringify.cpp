#include "Bencode_Lib_Tests.hpp"
#include "YAML_Stringify.hpp"

TEST_CASE("YAML stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify(makeStringify<YAML_Stringify>());
  SECTION("YAML stringify an integer (266).",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n266\n...\n");
  }
  SECTION("YAML stringify an integer (10000).",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n10000\n...\n");
  }
  SECTION("YAML stringify an integer (-10000).",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n-10000\n...\n");
  }
  SECTION("YAML stringify an string ('qwertyuiopas').",
          "[Bencode][Stringify][YAML][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"qwertyuiopas\"\n...\n");
  }
  SECTION("YAML stringify an string ('abcdefghijklmnopqrstuvwxyz')."
          "[Bencode][Stringify][YAML][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n\"abcdefghijklmnopqrstuvwxyz\"\n...\n");
  }
  SECTION("YAML stringify an empty string."
          "[Bencode][Stringify][YAML][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"\"\n...\n");
  }
  SECTION("YAML stringify an string with unprintable characters "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][YAML][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += 1;
    escaped += 2;
    BufferSource source{escaped};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n\"abcdefghijklmnopqrstuvwxyz\\u0000\\u0001\\u0002\"\n...\n");
  }
}
TEST_CASE("YAML stringify of collection types (list, dictionary) ",
          "[Bencode][Stringify][YAML]") {
  const Bencode bStringify(makeStringify<YAML_Stringify>());
  SECTION("YAML stringify an List of integers('li266ei6780ei88ee').",
          "[Bencode][Stringify][YAML][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n- 266\n- 6780\n- 88\n...\n");
  }
  SECTION("YAML stringify an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Stringify][YAML][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n- 266\n- \"five\"\n- 6780\n- \"one\"\n- 88\n...\n");
  }
  SECTION("YAML stringify an empty List.", "[Bencode][Stringify][YAML][List]") {
    BufferSource source{"le"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n[]\n...\n");
  }
  SECTION("YAML stringify an Dictionary of integers.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n\"one\": 1\n\"three\": 3\n\"two\": 2\n...\n");
  }
  SECTION("YAML stringify an Dictionary of strings.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n\"one\": \"0123456789\"\n\"three\": \"qwerty\"\n\"two\": "
            "\"asdfghjkl\"\n...\n");
  }
  SECTION("YAML stringify an Dictionary of arrays.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n\"one\": \n  - 1\n  - 2\n  - 3\n\"two\": \n  - 5555\n  - "
            "\"four\"\n...\n");
  }
  SECTION("YAML stringify an empty Dictionary.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"de"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n{}\n...\n");
  }
  SECTION("YAML stringify an empty List of empty Dictionaries.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"ldededededee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n- {}\n- {}\n- {}\n- {}\n- {}\n...\n");
  }
  SECTION("YAML stringify an list of empty lists.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferSource source{"llelelelelee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            "---\n- []\n- []\n- []\n- []\n- []\n...\n");
  }
  SECTION("YAML stringify integer zero.",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i0e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n0\n...\n");
  }
  SECTION("YAML stringify a negative integer.",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-1e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n-1\n...\n");
  }
  SECTION("YAML stringify max 64-bit integer.",
          "[Bencode][Stringify][YAML][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i9223372036854775807e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n9223372036854775807\n...\n");
  }
  SECTION("YAML stringify a single-string list.",
          "[Bencode][Stringify][YAML][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"l5:helloe"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n- \"hello\"\n...\n");
  }
  SECTION("YAML stringify a single-integer list.",
          "[Bencode][Stringify][YAML][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"li42ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n- 42\n...\n");
  }
  SECTION("YAML stringify a list of strings.",
          "[Bencode][Stringify][YAML][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"l3:foo3:bare"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n- \"foo\"\n- \"bar\"\n...\n");
  }
  SECTION("YAML stringify a single-entry dictionary with integer value.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d3:agei30ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"age\": 30\n...\n");
  }
  SECTION("YAML stringify a single-entry dictionary with string value.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d4:name5:Alicee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"name\": \"Alice\"\n...\n");
  }
  SECTION("YAML stringify a two-entry dictionary.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d1:ai1e1:bi2ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"a\": 1\n\"b\": 2\n...\n");
  }
  SECTION("YAML stringify a dictionary with a list value.",
          "[Bencode][Stringify][YAML][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d4:numsli1ei2eee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "---\n\"nums\": \n  - 1\n  - 2\n...\n");
  }
}
