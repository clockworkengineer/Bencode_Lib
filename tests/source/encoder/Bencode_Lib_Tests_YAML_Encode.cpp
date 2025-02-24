#include "Bencode_Lib_Tests.hpp"
#include "YAML_Encoder.hpp"

TEST_CASE("YAML encode of simple types (integer, string) ",
          "[Bencode][Encode]") {
  const Bencode bEncode(makeEncoder<YAML_Encoder>());
  SECTION("YAML encode an integer (266).", "[Bencode][Encode][YAML][Integer]") {
    BufferSource source{"i266e"};
    BufferDestination destination;
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE(destination.toString() ==
            "---\n266\n...\n");
  }
//   SECTION("YAML encode an integer (10000).", "[Bencode][Encode][YAML][Integer]") {
//     BufferSource source{"i10000e"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(destination.toString() ==
//             R"(<?xml version="1.0" encoding="UTF-8"?><root>10000</root>)");
//   }
//   SECTION("YAML encode an integer (-10000).",
//           "[Bencode][Encode][YAML][Integer]") {
//     BufferSource source{"i-10000e"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(destination.toString() ==
//             R"(<?xml version="1.0" encoding="UTF-8"?><root>-10000</root>)");
//   }
//   SECTION("YAML encode an string ('qwertyuiopas').",
//           "[Bencode][Encode][YAML][String]") {
//     BufferSource source{"12:qwertyuiopas"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root>qwertyuiopas</root>)");
//   }
//   SECTION("YAML encode an string ('abcdefghijklmnopqrstuvwxyz')."
//           "[Bencode][Encode][YAML][String]") {
//     BufferSource source{"26:abcdefghijklmnopqrstuvwxyz"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz</root>)");
//   }
//   SECTION("YAML encode an empty string."
//           "[Bencode][Encode][YAML][String]") {
//     BufferSource source{"0:"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(destination.toString() ==
//             R"(<?xml version="1.0" encoding="UTF-8"?><root></root>)");
//   }
//   SECTION("YAML encode an string with unprintable characters "
//           "('abcdefghijklmnopqrstuvwxyz').",
//           "[Bencode][Encode][YAML][String]") {
//     std::string escaped{"29:abcdefghijklmnopqrstuvwxyz"};
//     escaped += static_cast<char>(0);
//     escaped += static_cast<char>(1);
//     escaped += static_cast<char>(2);
//     BufferSource source{escaped};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz&#x0000;&#x0001;&#x0002;</root>)");
//   }
//   SECTION("YAML encode an string with escaped YAML characters "
//           "('abcdefghijklmnopqrstuvwxyz&<>'\"').",
//           "[Bencode][Encode][YAML][String]") {
//     std::string escaped{R"(32:abcdefghijklmnopqrstuvwxyz &<>'")"};
//     BufferSource source{escaped};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root>abcdefghijklmnopqrstuvwxyz &amp;&lt;&gt;&apos;&quot;</root>)");
//   }
// }
// TEST_CASE("YAML encode of collection types (list, dictionary) ",
//           "[Bencode][Encode][YAML]") {
//   const Bencode bEncode(makeEncoder<YAML_Encoder>());
//   SECTION("YAML encode an List of integers('li266ei6780ei88ee').",
//           "[Bencode][Encode][YAML][List]") {
//     BufferSource source{"li266ei6780ei88ee"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>6780</Row><Row>88</Row></root>)");
//   }
//   SECTION("YAML encode an List of integers and strings "
//           "('li266e4:fivei6780e3:onei88ee').",
//           "[Bencode][Encode][YAML][List]") {
//     BufferSource source{"li266e4:fivei6780e3:onei88ee"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><Row>266</Row><Row>five</Row><Row>6780</Row><Row>one</Row><Row>88</Row></root>)");
//   }
//   SECTION("YAML encode an empty List.", "[Bencode][Encode][YAML][List]") {
//     BufferSource source{"le"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><Row></Row></root>)");
//   }
//   SECTION("YAML encode an Dictionary of integers.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><one>1</one><three>3</three><two>2</two></root>)");
//   }
//   SECTION("YAML encode an Dictionary of strings.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><one>0123456789</one><three>qwerty</three><two>asdfghjkl</two></root>)");
//   }
//   SECTION("YAML encode an Dictionary of arrays.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"d3:oneli1ei2ei3ee3:twoli5555e4:fouree"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><one><Row>1</Row><Row>2</Row><Row>3</Row></one><two><Row>5555</Row><Row>four</Row></two></root>)");
//   }
//   SECTION("YAML encode an empty Dictionary.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"de"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(destination.toString() ==
//             R"(<?xml version="1.0" encoding="UTF-8"?><root></root>)");
//   }
//   SECTION("YAML encode an empty List of empty Dictionaries.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"ldededededee"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><Row></Row><Row></Row><Row></Row><Row></Row><Row></Row></root>)");
//   }
//   SECTION("YAML encode an list of empty lists.",
//           "[Bencode][Encode][YAML][Dictionary]") {
//     BufferSource source{"llelelelelee"};
//     BufferDestination destination;
//     bEncode.decode(source);
//     bEncode.encode(destination);
//     REQUIRE(
//         destination.toString() ==
//         R"(<?xml version="1.0" encoding="UTF-8"?><root><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row><Row><Row></Row></Row></root>)");
//   }
}