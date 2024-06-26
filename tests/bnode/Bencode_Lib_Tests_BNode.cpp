//
// Unit Tests: Bencode_Lib_Tests_BNode
//
// Description: BNode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;
// ==================
// BNode constructors
// ==================
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
// =================
// BNode assignments
// =================
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
// ==============
// BNode Indexing
// ==============
TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]") {
  const Bencode bEncode;
  SECTION("Decode dictionary and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(source);
    REQUIRE(BRef<String>((bEncode.root())["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())["three"]).value() == "qwerty");
  }
  SECTION("Decode list and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]).value() == "poiuytrewqas");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode list with embedded dictionary and check its components using "
          "indexing",
          "[Bencode][BNode][Index]") {
    BufferSource source{"l6:sillyyd3:one10:01234567895:three6:qwerty3:"
                               "two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())[1]["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())[1]["three"]).value() == "qwerty");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode dictionary and check an invalid key generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(source);
    REQUIRE_THROWS_AS(isA<Dictionary>(bEncode.root()["onee"]), Dictionary::Error);
    REQUIRE_THROWS_WITH(isA<Dictionary>(bEncode.root()["onee"]),
                        "Dictionary Error: Invalid key used in dictionary.");
  }
  SECTION("Decode list and check an invalid index generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    REQUIRE_THROWS_AS(isA<List>(bEncode.root()[3]), List::Error);
    REQUIRE_THROWS_WITH(isA<List>(bEncode.root()[3]),
                        "List Error: Invalid index used in list.");
  }
}
TEST_CASE("Check BNode reference functions work.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bEncode.decode(source);
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{"10:0123456789"};
    bEncode.decode(source);
    REQUIRE(BRef<String>((bEncode.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(source);
    REQUIRE(BRef<List>((bEncode.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]") {
    BufferSource source{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(source);
    REQUIRE(BRef<Dictionary>((bEncode.root())).size() == 4);
  }
  SECTION("BRef check node type correct.", "[Bencode][BNode][Reference]") {
    BufferSource source{"i45500e"};
    bEncode.decode(source);
    REQUIRE_THROWS_AS(BRef<Dictionary>(bEncode.root()), BNode::Error);
    REQUIRE_THROWS_WITH(BRef<Dictionary>(bEncode.root()),
                        "BNode Error: Node not a dictionary.");
  }
}