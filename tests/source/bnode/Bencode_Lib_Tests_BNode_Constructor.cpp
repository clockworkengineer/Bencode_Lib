#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check use of BNode constructors.", "[Bencode][BNode][Constructor]") {
  SECTION("Construct BNode(integer).",
          "[Bencode][BNode][Constructor][Integer]") {
    BNode bNode(999);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 999);
  }
  SECTION("Construct BNode(long).", "[Bencode][BNode][Constructor][Long]") {
    BNode bNode(99988899l);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 99988899l);
  }
  SECTION("Construct BNode(long long).",
          "[Bencode][BNode][Constructor][Long Long]") {
    BNode bNode(99988899ll);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 99988899ll);
  }

  SECTION("Construct BNode(C string).",
          "[Bencode][BNode][Constructor][CString]") {
    BNode bNode("test string");
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  SECTION("Construct BNode(string).", "[Bencode][BNode][Constructor][String]") {
    std::string string{"test string"};
    BNode bNode(string);
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  SECTION("Construct BNode(list).", "[Bencode][BNode][Constructor][List]") {
    BNode bNode{1, 2, 3, 4};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = BRef<List>(bNode).value();
    REQUIRE(BRef<Integer>(list[0]).value() == 1);
    REQUIRE(BRef<Integer>(list[1]).value() == 2);
    REQUIRE(BRef<Integer>(list[2]).value() == 3);
    REQUIRE(BRef<Integer>(list[3]).value() == 4);
  }
  SECTION("Construct BNode(dictionary).",
          "[Bencode][BNode][Constructor][Dictionary]") {
    BNode bNode{{"key1", 55}, {"key2", 26666}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = BRef<Dictionary>(bNode);
    REQUIRE(BRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(BRef<Integer>(dictionary["key2"]).value() == 26666);
  }
  SECTION("Construct BNode(list with nested list).",
          "[Bencode][BNode][Constructor][Array]") {
    BNode bNode{1, 2, 3, 4, BNode{5, 6, 7, 8}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = BRef<List>(bNode).value();
    REQUIRE(BRef<Integer>(list[0]).value() == 1);
    REQUIRE(BRef<Integer>(list[1]).value() == 2);
    REQUIRE(BRef<Integer>(list[2]).value() == 3);
    REQUIRE(BRef<Integer>(list[3]).value() == 4);
    REQUIRE(BRef<Integer>(list[4][0]).value() == 5);
    REQUIRE(BRef<Integer>(list[4][1]).value() == 6);
    REQUIRE(BRef<Integer>(list[4][2]).value() == 7);
    REQUIRE(BRef<Integer>(list[4][3]).value() == 8);
  }
  SECTION("Construct BNode(dictionary with nested dictionary).",
          "[Bencode][BNode][Constructor][Dictionary]") {
    BNode bNode{{"key1", 55},
                {"key2", 26666},
                {"key3", BNode{{"key4", 5555}, {"key5", 7777}}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = BRef<Dictionary>(bNode);
    REQUIRE(BRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(BRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(BRef<Integer>(dictionary["key3"]["key4"]).value() == 5555);
    REQUIRE(BRef<Integer>(dictionary["key3"]["key5"]).value() == 7777);
  }
  SECTION("Construct BNode(list with nested dictionary).",
          "[Bencode][BNode][Constructor][List]") {
    BNode bNode{1, 2, 3, 4, BNode{{"key4", 5555}, {"key5", 7777}}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = BRef<List>(bNode).value();
    REQUIRE(BRef<Integer>(list[0]).value() == 1);
    REQUIRE(BRef<Integer>(list[1]).value() == 2);
    REQUIRE(BRef<Integer>(list[2]).value() == 3);
    REQUIRE(BRef<Integer>(list[3]).value() == 4);
    REQUIRE(BRef<Integer>(list[4]["key4"]).value() == 5555);
    REQUIRE(BRef<Integer>(list[4]["key5"]).value() == 7777);
  }
  SECTION("Construct BNode(dictionary with nested list).",
          "[Bencode][BNode][Constructor][Dictionary]") {
    BNode bNode{{"key1", 55}, {"key2", 26666}, {"key3", BNode{5, 6, 7, 8}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = BRef<Dictionary>(bNode);
    REQUIRE(BRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(BRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(BRef<Integer>(dictionary["key3"][0]).value() == 5);
    REQUIRE(BRef<Integer>(dictionary["key3"][1]).value() == 6);
    REQUIRE(BRef<Integer>(dictionary["key3"][2]).value() == 7);
    REQUIRE(BRef<Integer>(dictionary["key3"][3]).value() == 8);
  }
}