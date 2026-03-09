#include "Bencode_Lib_Tests.hpp"
#include "JSON_Stringify.hpp"

TEST_CASE("JSON stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify(makeStringify<JSON_Stringify>());
  SECTION("JSON stringify an integer (266).",
          "[Bencode][Stringify][JSON][Integer]") {
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
  SECTION("JSON stringify an empty string.",
          "[Bencode][Stringify][JSON][String]") {
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
  SECTION("JSON stringify an empty List).",
          "[Bencode][Stringify][JSON][List]") {
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
  SECTION("JSON stringify integer zero.",
          "[Bencode][Stringify][JSON][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i0e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "0");
  }
  SECTION("JSON stringify a single-element list.",
          "[Bencode][Stringify][JSON][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"li99ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([99])");
  }
  SECTION("JSON stringify a list of strings.",
          "[Bencode][Stringify][JSON][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"l3:foo3:bare"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"(["foo","bar"])");
  }
  SECTION("JSON stringify nested lists.", "[Bencode][Stringify][JSON][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"lli1ei2eeli3ei4eee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([[1,2],[3,4]])");
  }
  SECTION("JSON stringify dictionary with list value.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d4:numsli1ei2ei3eee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"({"nums" : [1,2,3]})");
  }
  SECTION("JSON stringify dictionary with nested dictionary.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d5:outerd5:inneri42eee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"({"outer" : {"inner" : 42}})");
  }
  SECTION("JSON stringify list containing a dictionary.",
          "[Bencode][Stringify][JSON][List]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"ld3:keyi7eee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"([{"key" : 7}])");
  }
  SECTION("JSON stringify single-entry dictionary with string value.",
          "[Bencode][Stringify][JSON][Dictionary]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d4:name5:Alicee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == R"({"name" : "Alice"})");
  }
  SECTION("JSON stringify negative integer.",
          "[Bencode][Stringify][JSON][Integer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-1e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "-1");
  }
}