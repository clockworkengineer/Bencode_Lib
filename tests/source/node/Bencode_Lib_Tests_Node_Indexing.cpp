#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Use of Node indexing operators", "[Bencode][Node][Index]") {
  const Bencode bStringify;
  SECTION("Parse dictionary and check its components using indexing",
          "[Bencode][Node][Index]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE(NRef<String>((bStringify.root())["one"]).value() == "0123456789");
    REQUIRE(NRef<String>((bStringify.root())["two"]).value() == "asdfghjkl");
    REQUIRE(NRef<String>((bStringify.root())["three"]).value() == "qwerty");
  }
  SECTION("Parse list and check its components using indexing",
          "[Bencode][Node][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(NRef<String>((bStringify.root())[0]).value() == "sillyy");
    REQUIRE(NRef<String>((bStringify.root())[1]).value() == "poiuytrewqas");
    REQUIRE(NRef<String>((bStringify.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse list with embedded dictionary and check its components using "
          "indexing",
          "[Bencode][Node][Index]") {
    BufferSource source{"l6:sillyyd3:one10:01234567895:three6:qwerty3:"
                        "two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE(NRef<String>((bStringify.root())[0]).value() == "sillyy");
    REQUIRE(NRef<String>((bStringify.root())[1]["one"]).value() ==
            "0123456789");
    REQUIRE(NRef<String>((bStringify.root())[1]["two"]).value() == "asdfghjkl");
    REQUIRE(NRef<String>((bStringify.root())[1]["three"]).value() == "qwerty");
    REQUIRE(NRef<String>((bStringify.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Parse dictionary and check an invalid key generates exception",
          "[Bencode][Node][Index]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(isA<Dictionary>(bStringify.root()["onee"]), Node::Error);
    REQUIRE_THROWS_WITH(isA<Dictionary>(bStringify.root()["onee"]),
                        "Node Error: Invalid key used in dictionary.");
  }
  SECTION("Parse list and check an invalid index generates exception",
          "[Bencode][Node][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    REQUIRE_THROWS_AS(isA<List>(bStringify.root()[3]), Node::Error);
    REQUIRE_THROWS_WITH(isA<List>(bStringify.root()[3]),
                        "Node Error: Invalid index used in list.");
  }
  SECTION("Parse nested dictionary and index two levels deep.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"d5:outerd5:inner5:helloee"});
    REQUIRE(NRef<String>(bStringify.root()["outer"]["inner"]).value() ==
            "hello");
  }
  SECTION("Parse list of integers and verify each element by index.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"li10ei20ei30ee"});
    REQUIRE(NRef<Integer>(bStringify.root()[0]).value() == 10);
    REQUIRE(NRef<Integer>(bStringify.root()[1]).value() == 20);
    REQUIRE(NRef<Integer>(bStringify.root()[2]).value() == 30);
  }
  SECTION("Parse dictionary with integer values and verify by key.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"d1:ai1e1:bi2e1:ci3ee"});
    REQUIRE(NRef<Integer>(bStringify.root()["a"]).value() == 1);
    REQUIRE(NRef<Integer>(bStringify.root()["b"]).value() == 2);
    REQUIRE(NRef<Integer>(bStringify.root()["c"]).value() == 3);
  }
  SECTION("Parse dictionary containing a list and index into it.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"d4:numsli1ei2ei3eee"});
    REQUIRE(isA<List>(bStringify.root()["nums"]));
    REQUIRE(NRef<Integer>(bStringify.root()["nums"][0]).value() == 1);
    REQUIRE(NRef<Integer>(bStringify.root()["nums"][1]).value() == 2);
    REQUIRE(NRef<Integer>(bStringify.root()["nums"][2]).value() == 3);
  }
  SECTION("Parse list containing a dictionary and index into it.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"ld3:keyi99eee"});
    REQUIRE(isA<Dictionary>(bStringify.root()[0]));
    REQUIRE(NRef<Integer>(bStringify.root()[0]["key"]).value() == 99);
  }
  SECTION("Index into dictionary using string_view key.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"d4:name5:Nielse"});
    const std::string_view key{"name"};
    REQUIRE(NRef<String>(bStringify.root()[key]).value() == "Niels");
  }
  SECTION("Const reference indexing on dictionary returns correct value.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"d5:valuei42ee"});
    const Node &root = bStringify.root();
    REQUIRE(NRef<Integer>(root["value"]).value() == 42);
  }
  SECTION("Const reference indexing on list returns correct value.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"li7ei8ei9ee"});
    const Node &root = bStringify.root();
    REQUIRE(NRef<Integer>(root[0]).value() == 7);
    REQUIRE(NRef<Integer>(root[1]).value() == 8);
    REQUIRE(NRef<Integer>(root[2]).value() == 9);
  }
  SECTION("Three levels of list indexing.", "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"llli42eeee"});
    REQUIRE(NRef<Integer>(bStringify.root()[0][0][0]).value() == 42);
  }
  SECTION("Index 0 on single-element list returns its only item.",
          "[Bencode][Node][Index]") {
    bStringify.parse(BufferSource{"l6:onlymee"});
    REQUIRE(NRef<String>(bStringify.root()[0]).value() == "onlyme");
  }
}