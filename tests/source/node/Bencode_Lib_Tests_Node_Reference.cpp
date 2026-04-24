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
  SECTION("NRef on List throws when node is a String.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"5:hello"});
    REQUIRE_THROWS_AS(NRef<List>(bStringify.root()), Node::Error);
    REQUIRE_THROWS_WITH(NRef<List>(bStringify.root()),
                        "Node Error: Node not a list.");
  }
  SECTION("NRef on Integer throws when node is a List.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"li1ei2ee"});
    REQUIRE_THROWS_AS(NRef<Integer>(bStringify.root()), Node::Error);
    REQUIRE_THROWS_WITH(NRef<Integer>(bStringify.root()),
                        "Node Error: Node not an integer.");
  }
  SECTION("NRef on String throws when node is an Integer.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"i99e"});
    REQUIRE_THROWS_AS(NRef<String>(bStringify.root()), Node::Error);
    REQUIRE_THROWS_WITH(NRef<String>(bStringify.root()),
                        "Node Error: Node not a string.");
  }
  SECTION("Integer reference returns correct negative value.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"i-9999e"});
    REQUIRE(NRef<Integer>(bStringify.root()).value() == -9999);
  }
  SECTION("String reference returns correct value for short string.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"1:x"});
    REQUIRE(NRef<String>(bStringify.root()).value() == "x");
  }
  SECTION("String reference returns correct value for empty string.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"0:"});
    REQUIRE(NRef<String>(bStringify.root()).value().empty());
  }
  SECTION("List reference reports correct size for single element.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"li42ee"});
    REQUIRE(NRef<List>(bStringify.root()).size() == 1);
  }
  SECTION("List reference element access returns correct node.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"li10ei20ei30ee"});
    auto &lst = NRef<List>(bStringify.root());
    REQUIRE(lst.size() == 3);
    REQUIRE(NRef<Integer>(lst[0]).value() == 10);
    REQUIRE(NRef<Integer>(lst[1]).value() == 20);
    REQUIRE(NRef<Integer>(lst[2]).value() == 30);
  }
  SECTION("Dictionary reference reports correct size.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"d1:ai1e1:bi2ee"});
    REQUIRE(NRef<Dictionary>(bStringify.root()).size() == 2);
  }
  SECTION("Dictionary reference key lookup returns correct node.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"d3:key5:valuee"});
    auto &dict = NRef<Dictionary>(bStringify.root());
    REQUIRE(NRef<String>(dict["key"]).value() == "value");
  }
  SECTION("Dictionary insertion keeps keys sorted and rejects duplicates.",
          "[Bencode][Node][Reference]") {
    Node dictNode = Node::make<Dictionary>();
    auto &dict = NRef<Dictionary>(dictNode);
    dict.add(Dictionary::Entry("b", Node::make<Integer>(2)));
    dict.add(Dictionary::Entry("a", Node::make<Integer>(1)));
    REQUIRE(dict.size() == 2);
    REQUIRE(dict.value()[0].getKey() == "a");
    REQUIRE(dict.value()[1].getKey() == "b");
    REQUIRE(NRef<Integer>(dict["a"]).value() == 1);
    REQUIRE(NRef<Integer>(dict["b"]).value() == 2);
    REQUIRE_THROWS_AS(dict.add(Dictionary::Entry("a", Node::make<Integer>(3))),
                      Node::Error);
    REQUIRE_THROWS_WITH(
        dict.add(Dictionary::Entry("a", Node::make<Integer>(3))),
        "Node Error: Duplicate dictionary key.");
  }
  SECTION("Const NRef on Integer returns correct value.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"i7e"});
    const Node &root = bStringify.root();
    REQUIRE(NRef<const Integer>(root).value() == 7);
  }
  SECTION("Const NRef on String returns correct value.",
          "[Bencode][Node][Reference]") {
    bStringify.parse(BufferSource{"4:test"});
    const Node &root = bStringify.root();
    REQUIRE(NRef<const String>(root).value() == "test");
  }
}