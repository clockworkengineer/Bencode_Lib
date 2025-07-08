#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check use of Node assigment operators.",
          "[Bencode][Node][Assignment]") {
  Node bNode;
  SECTION("Assign integer to Node.", "[Bencode][Node][Assignment][Integer]") {
    bNode = 999;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 999);
  }
  SECTION("Assign long to Node.", "[Bencode][Node][Assignment][Long]") {
    bNode = 99988899l;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 99988899l);
  }
  SECTION("Assign long long to Node.",
          "[Bencode][Node][Assignment][Long Long]") {
    bNode = 99988899ll;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 99988899ll);
  }
  SECTION("Assign float to Node.", "[Bencode][Node][Assignment][Float]") {
    bNode = 777.999f;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 777);
  }
  SECTION("Assign double to Node.", "[Bencode][Node][Assignment][Double]") {
    bNode = 66666.8888;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 66666);
  }
  SECTION("Assign long double to Node.",
          "[Bencode][Node][Assignment][Long Double]") {
    bNode = 66666.8888l;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 66666);
  }
  SECTION("Assign C string to Node.",
          "[Bencode][Node][Assignment][CString]") {
    bNode = "test string";
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(NRef<String>(bNode).value() == "test string");
  }
  SECTION("Assign string to Node.", "[Bencode][Node][Assignment][String]") {
    bNode = std::string("test string");
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(NRef<String>(bNode).value() == "test string");
  }
  SECTION("Assign boolean to Node.", "[Bencode][Node][Assignment][Boolean]") {
    bNode = true;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 1);
  }
  SECTION("Assign nullptr to Node.", "[Bencode][Node][Assignment][Null]") {
    bNode = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 0);
  }
  SECTION("Assign list to Node.", "[Bencode][Node][Assignment][List]") {
    bNode = {1, 2, 3, 4};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(list[0]).value() == 1);
    REQUIRE(NRef<Integer>(list[1]).value() == 2);
    REQUIRE(NRef<Integer>(list[2]).value() == 3);
    REQUIRE(NRef<Integer>(list[3]).value() == 4);
  }
  SECTION("Assign dictionary to Node.",
          "[Bencode][Node][Assignment][Dictionary]") {
    bNode = {{"key1", 55}, {"key2", 26666}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
  }
  SECTION("Assign Node with list with nested list.",
          "[Bencode][Node][Constructor][List]") {
    bNode = {1, 2, 3, 4, Node{5, 6, 7, 8}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(list[0]).value() == 1);
    REQUIRE(NRef<Integer>(list[1]).value() == 2);
    REQUIRE(NRef<Integer>(list[2]).value() == 3);
    REQUIRE(NRef<Integer>(list[3]).value() == 4);
    REQUIRE(NRef<Integer>(list[4][0]).value() == 5);
    REQUIRE(NRef<Integer>(list[4][1]).value() == 6);
    REQUIRE(NRef<Integer>(list[4][2]).value() == 7);
    REQUIRE(NRef<Integer>(list[4][3]).value() == 8);
  }
  SECTION("Assign Node with dictionary with a nested dictionary.",
          "[Bencode][Node][Constructor][Dictionary]") {
    bNode = {{"key1", 55},
             {"key2", 26666},
             {"key3", Node{{"key4", 5555}, {"key5", 7777}}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(NRef<Integer>(dictionary["key3"]["key4"]).value() == 5555);
    REQUIRE(NRef<Integer>(dictionary["key3"]["key5"]).value() == 7777);
  }
  SECTION("Assign Node with list with a nested dictionary.",
          "[Bencode][Node][Constructor][List]") {
    bNode = {1, 2, 3, 4, Node{{"key4", 5555}, {"key5", 7777}}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(list[0]).value() == 1);
    REQUIRE(NRef<Integer>(list[1]).value() == 2);
    REQUIRE(NRef<Integer>(list[2]).value() == 3);
    REQUIRE(NRef<Integer>(list[3]).value() == 4);
    REQUIRE(NRef<Integer>(list[4]["key4"]).value() == 5555);
    REQUIRE(NRef<Integer>(list[4]["key5"]).value() == 7777);
  }
  SECTION("Assign Node with dictionary with nested a list.",
          "[Bencode][Node][Constructor][Object]") {
    bNode = {{"key1", 55}, {"key2", 26666}, {"key3", Node{5, 6, 7, 8}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(NRef<Integer>(dictionary["key3"][0]).value() == 5);
    REQUIRE(NRef<Integer>(dictionary["key3"][1]).value() == 6);
    REQUIRE(NRef<Integer>(dictionary["key3"][2]).value() == 7);
    REQUIRE(NRef<Integer>(dictionary["key3"][3]).value() == 8);
  }
}