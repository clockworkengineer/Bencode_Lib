#include "Bencode_Lib_Tests.hpp"
#include "XML_Stringify.hpp"

TEST_CASE("XML stringify of simple types (integer, string) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify(makeStringify<XML_Stringify>());
  SECTION("XML stringify an integer (266).", "[Bencode][Stringify][XML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>266</root>)");
  }
  SECTION("XML stringify an integer (10000).", "[Bencode][Stringify][XML][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>10000</root>)");
  }
  SECTION("XML stringify an integer (-10000).",
          "[Bencode][Stringify][XML][Integer]") {
    BufferSource source{"i-10000e"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>-10000</root>)");
  }
  SECTION("XML stringify an string ('qwertyuiopas').",
          "[Bencode][Stringify][XML][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>qwertyuiopas</root>)");
  }
  SECTION("XML stringify an string ('abcdefghijklmnopqrstuvwxyz')."
          "[Bencode][Stringify][XML][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz</root>)");
  }
  SECTION("XML stringify an empty string."
          "[Bencode][Stringify][XML][String]") {
    BufferSource source{"0:"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root></root>)");
  }
  SECTION("XML stringify an string with unprintable characters "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Stringify][XML][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += 1;
    escaped += 2;
    BufferSource source{escaped};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0000;&#x0001;&#x0002;</root>)");
  }
  SECTION("XML stringify an string with escaped XML characters "
          "('abcdefghijklmnopqrstuvwxyz&<>'\"').",
          "[Bencode][Stringify][XML][String]") {
    std::string escaped{R"(32:abcdefghijklmnopqrstuvwxyz &<>'")"};
    BufferSource source{escaped};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &amp;&lt;&gt;&apos;&quot;</root>)");
  }
}
TEST_CASE("XML stringify of collection types (list, dictionary) ",
          "[Bencode][Stringify][XML]") {
  const Bencode bStringify(makeStringify<XML_Stringify>());
  SECTION("XML stringify an List of integers('li266ei6780ei88ee').",
          "[Bencode][Stringify][XML][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>6780</Row><Row>88</Row></root>)");
  }
  SECTION("XML stringify an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Stringify][XML][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>five</Row><Row>6780</Row><Row>one</Row><Row>88</Row></root>)");
  }
  SECTION("XML stringify an empty List.", "[Bencode][Stringify][XML][List]") {
    BufferSource source{"le"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row></Row></root>)");
  }
  SECTION("XML stringify an Dictionary of integers.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>1</one><three>3</three><two>2</two></root>)");
  }
  SECTION("XML stringify an Dictionary of strings.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>0123456789</one><three>qwerty</three><two>asdfghjkl</two></root>)");
  }
  SECTION("XML stringify an Dictionary of arrays.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one><Row>1</Row><Row>2</Row><Row>3</Row></one><two><Row>5555</Row><Row>four</Row></two></root>)");
  }
  SECTION("XML stringify an empty Dictionary.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"de"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root></root>)");
  }
  SECTION("XML stringify an empty List of empty Dictionaries.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"ldededededee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row></Row><Row></Row><Row></Row><Row></Row><Row></Row></root>)");
  }
  SECTION("XML stringify an list of empty lists.",
          "[Bencode][Stringify][XML][Dictionary]") {
    BufferSource source{"llelelelelee"};
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row></root>)");
  }
}