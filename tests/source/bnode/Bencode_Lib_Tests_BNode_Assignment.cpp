#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check use of BNode assigment operators.",
          "[Bencode][BNode][Assignment]") {
  BNode bNode;
  SECTION("Assign integer to BNode.", "[Bencode][BNode][Assignment][Integer]") {
    bNode = 999;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 999);
  }
  SECTION("Assign long to BNode.", "[Bencode][BNode][Assignment][Long]") {
    bNode = 99988899l;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 99988899l);
  }
  SECTION("Assign long long to BNode.",
          "[Bencode][BNode][Assignment][Long\\ Long]") {
    bNode = 99988899ll;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 99988899ll);
  }
  SECTION("Assign float to BNode.", "[Bencode][BNode][Assignment][Float]") {
    bNode = 777.999f;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 777);
  }
  SECTION("Assign double to BNode.", "[Bencode][BNode][Assignment][Double]") {
    bNode = 66666.8888;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 66666);
  }
  SECTION("Assign long double to BNode.",
          "[Bencode][BNode][Assignment][Long Double]") {
    bNode = 66666.8888l;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 66666);
  }
  SECTION("Assign C string to BNode.",
          "[Bencode][BNode][Assignment][CString]") {
    bNode = "test string";
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  SECTION("Assign string to BNode.", "[Bencode][BNode][Assignment][String]") {
    bNode = std::string("test string");
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  SECTION("Assign boolean to BNode.", "[Bencode][BNode][Assignment][Boolean]") {
    bNode = true;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 1);
  }
  SECTION("Assign nullptr to BNode.", "[Bencode][BNode][Assignment][Null]") {
    bNode = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(BRef<Integer>(bNode).value() == 0);
  }
  SECTION("Assign list to BNode.", "[Bencode][BNode][Assignment][List]") {
    bNode = {1, 2, 3, 4};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = BRef<List>(bNode).value();
    REQUIRE(BRef<Integer>(list[0]).value() == 1);
    REQUIRE(BRef<Integer>(list[1]).value() == 2);
    REQUIRE(BRef<Integer>(list[2]).value() == 3);
    REQUIRE(BRef<Integer>(list[3]).value() == 4);
  }
  SECTION("Assign dictionary to BNode.",
          "[Bencode][BNode][Assignment][Dictionary]") {
    bNode = {{"key1", 55}, {"key2", 26666}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = BRef<Dictionary>(bNode);
    REQUIRE(BRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(BRef<Integer>(dictionary["key2"]).value() == 26666);
  }
  SECTION("Assign BNode with list with nested list.",
          "[Bencode][BNode][Constructor][List]") {
    bNode = {1, 2, 3, 4, BNode{5, 6, 7, 8}};
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
  SECTION("Assign BNode with dictionary with a nested dictionary.",
          "[Bencode][BNode][Constructor][Dictionary]") {
    bNode = {{"key1", 55},
             {"key2", 26666},
             {"key3", BNode{{"key4", 5555}, {"key5", 7777}}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = BRef<Dictionary>(bNode);
    REQUIRE(BRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(BRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(BRef<Integer>(dictionary["key3"]["key4"]).value() == 5555);
    REQUIRE(BRef<Integer>(dictionary["key3"]["key5"]).value() == 7777);
  }
  SECTION("Assign BNode with list with a nested dictionary.",
          "[Bencode][BNode][Constructor][List]") {
    bNode = {1, 2, 3, 4, BNode{{"key4", 5555}, {"key5", 7777}}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = BRef<List>(bNode).value();
    REQUIRE(BRef<Integer>(list[0]).value() == 1);
    REQUIRE(BRef<Integer>(list[1]).value() == 2);
    REQUIRE(BRef<Integer>(list[2]).value() == 3);
    REQUIRE(BRef<Integer>(list[3]).value() == 4);
    REQUIRE(BRef<Integer>(list[4]["key4"]).value() == 5555);
    REQUIRE(BRef<Integer>(list[4]["key5"]).value() == 7777);
  }
  SECTION("Assign BNode with dictionary with nested a list.",
          "[Bencode][BNode][Constructor][Object]") {
    bNode = {{"key1", 55}, {"key2", 26666}, {"key3", BNode{5, 6, 7, 8}}};
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