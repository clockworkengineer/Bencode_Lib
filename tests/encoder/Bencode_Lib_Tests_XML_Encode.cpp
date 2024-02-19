//
// Unit Tests: Bencode_Lib_Tests_JSON_Encode
//
// Description: XML encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("XML encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode(std::make_unique<XML_Encoder>().release());
  SECTION("XML encode an integer (266).", "[Bencode][Encode][XML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>266</root>)");
  }
  SECTION("XML encode an integer (10000).", "[Bencode][Encode][XML][Integer]") {
    BufferSource source{"i10000e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            R"(<?xml version="1.0" encoding="UTF-8"?><root>10000</root>)");
  }
  SECTION("XML encode an string ('qwertyuiopas').",
          "[Bencode][Encode][XML][String]") {
    BufferSource source{"12:qwertyuiopas"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>qwertyuiopas</root>)");
  }
  SECTION("XML encode an string ('abcdefghijklmnopqrstuvwxyz')."
          "[Bencode][Encode][XML][String]") {
    BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz</root>)");
  }
  SECTION("XML encode an string with unprintable characters "
          "('abcdefghijklmnopqrstuvwxyz').",
          "[Bencode][Encode][JSON][String]") {
    std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
    escaped += static_cast<char>(0);
    escaped += static_cast<char>(1);
    escaped += static_cast<char>(2);
    BufferSource source{escaped};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0000;&#x0001;&#x0002;</root>)");
  }
  SECTION("XML encode an string with escaped XML characters "
          "('abcdefghijklmnopqrstuvwxyz&<>'\"').",
          "[Bencode][Encode][JSON][String]") {
    std::string escaped{R"(32:abcdefghijklmnopqrstuvwxyz &<>'")"};
    BufferSource source{escaped};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &amp;&lt;&gt;&apos;&quot;</root>)");
  }
}
TEST_CASE("XML encode of collection types (list, dictionary) ",
          "[Bencode][Encode][XML]") {
  const Bencode bEncode(std::make_unique<XML_Encoder>().release());
  SECTION("XML encode an List of integers('li266ei6780ei88ee').",
          "[Bencode][Encode][XML][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>6780</Row><Row>88</Row></root>)");
  }
  SECTION("XML encode an List of integers and strings "
          "('li266e4:fivei6780e3:onei88ee').",
          "[Bencode][Encode][XML][List]") {
    BufferSource source{"li266e4:fivei6780e3:onei88ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>five</Row><Row>6780</Row><Row>one</Row><Row>88</Row></root>)");
  }
  SECTION("XML encode an Dictionary of integers.",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>1</one><three>3</three><two>2</two></root>)");
  }
  SECTION("XML encode an Dictionary of strings.",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one>0123456789</one><three>qwerty</three><two>asdfghjkl</two></root>)");
  }
  SECTION("XML encode an Dictionary of arrays.",
          "[Bencode][Encode][XML][Dictionary]") {
    BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(
        destination.toString() ==
        R"(<?xml version="1.0" encoding="UTF-8"?><root><one><Row>1</Row><Row>2</Row><Row>3</Row></one><two><Row>5555</Row><Row>four</Row></two></root>)");
  }
}