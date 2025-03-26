#include "Bencode_Lib_Tests.hpp"
#include "JSON_Stringify.hpp"

TEST_CASE("JSON stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify(makeStringify<JSON_Stringify>());
  SECTION("JSON stringify an integer (266).", "[Bencode][Stringify][JSON][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "266");
  }
  SECTION("JSON stringify an integer (10000).",
          "[Bencode][Stringify][JSON][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "10000");
  }
  SECTION("JSON stringify an integer (-10000).",
          "[Bencode][Stringify][JSON][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "-10000");
  }
  SECTION("JSON stringify an string ('qwertyuiopas').",
          "[Bencode][Stringify][JSON][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"("qwertyuiopas")");
  }
  SECTION("JSON stringify an string ('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][JSON][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"("abcdefghijklmnopqrstuvwxyz")");
  }
  SECTION("JSON stringify an empty string.", "[Bencode][Stringify][JSON][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"("")");
  }
  SECTION("JSON stringify an string with unprintable character. "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][JSON][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += 1;
    escaped += 2;
    BufferSource source{escaped};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"("abcdefghijklmnopqrstuvwxyz\u0000\u0001\u0002")");
  }
}
TEST_CASE("JSON stringify of collection types (list, dictionary) ",
          "[Bencode][Stringify][JSON]") {
  const Bencode bStringify(makeStringify<JSON_Stringify>());
  SECTION("JSON stringify an List of integers('li266ei6780ei88ee').",
          "[Bencode][Stringify][JSON][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([266,6780,88])");
  }
  SECTION("JSON stringify an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Stringify][JSON][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([266,"five",6780,"one",88])");
  }
  SECTION("JSON stringify an empty List).", "[Bencode][Stringify][JSON][List]") {
    BufferSource source{"le"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([])");
  }
  SECTION("JSON stringify an Dictionary of integers.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"({"one" : 1,"three" : 3,"two" : 2})");
  }
  SECTION("JSON stringify an Dictionary of strings.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"({"one" : "0123456789","three" : "qwerty","two" : "asdfghjkl"})");
  }
  SECTION("JSON stringify an Dictionary of arrays.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"({"one" : [1,2,3],"two" : [5555,"four"]})");
  }
  SECTION("JSON stringify an empty Dictionary.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"de"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"({})");
  }
  SECTION("JSON stringify an empty List of empty Dictionaries.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"ldededededee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([{},{},{},{},{}])");
  }
  SECTION("JSON stringify an empty List of empty lists.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferSource source{"llelelelelee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([[],[],[],[],[]])");
  }
}