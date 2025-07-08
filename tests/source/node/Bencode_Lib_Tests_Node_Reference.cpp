#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check Node reference functions work.",
          "[Bencode][Node][Reference]") {
  const Bencode bStringify;
  SECTION("Integer reference.", "[Bencode][Node][Reference]") {
    BufferSource source{"i45500e"};
    bStringify.parse(source);
    REQUIRE(NRef<Integer>((bStringify.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][Node][Reference]") {
    BufferSource source{"10:0123456789"};
    bStringify.parse(source);
    REQUIRE(NRef<String>((bStringify.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][Node][Reference]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(NRef<List>((bStringify.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][Node][Reference]") {
    BufferSource source{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE(NRef<Dictionary>((bStringify.root())).size() == 4);
  }
  SECTION("NRef check node type correct.", "[Bencode][Node][Reference]") {
    BufferSource source{"i45500e"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(NRef<Dictionary>(bStringify.root()), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Dictionary>(bStringify.root()),
                        "Node Error: Node not a dictionary.");
  }
}