//
// Unit Tests: Bencode
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
    REQUIRE_FALSE(!bNode.isInteger());
    REQUIRE(BRef<Integer>(bNode).value() == 999);
  }
  SECTION("Construct BNode(long).", "[Bencode][BNode][Constructor][Long]") {
    BNode bNode(99988899l);
    REQUIRE_FALSE(!bNode.isInteger());
    REQUIRE(BRef<Integer>(bNode).value() == 99988899l);
  }
  SECTION("Construct BNode(long long).",
          "[Bencode][BNode][Constructor][Long Long]") {
    BNode bNode(99988899ll);
    REQUIRE_FALSE(!bNode.isInteger());
    REQUIRE(BRef<Integer>(bNode).value() == 99988899ll);
  }

  SECTION("Construct BNode(C string).",
          "[Bencode][BNode][Constructor][CString]") {
    BNode bNode("test string");
    REQUIRE_FALSE(!bNode.isString());
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  SECTION("Construct BNode(string).", "[Bencode][BNode][Constructor][String]") {
    std::string string {"test string"};
    BNode bNode(string);
    REQUIRE_FALSE(!bNode.isString());
    REQUIRE(BRef<String>(bNode).value() == "test string");
  }
  //   SECTION("Construct BNode(boolean).",
  //   "[Bencode][BNode][Constructor][Boolean]")
  //   {
  //     BNode bNode(false);
  //     REQUIRE_FALSE(!bNode.isBoolean());
  //     REQUIRE_FALSE(BRef<Boolean>(bNode).value());
  //   }
  //   SECTION("Construct BNode(null).", "[Bencode][BNode][Constructor][Null]")
  //   {
  //     BNode bNode(nullptr);
  //     REQUIRE_FALSE(!bNode.isNull());
  //     REQUIRE(BRef<Null>(bNode).getNull() == nullptr);
  //   }
  //   SECTION("Construct BNode(array).",
  //   "[Bencode][BNode][Constructor][Array]")
  //   {
  //     BNode bNode{ 1, 2, 3, 4 };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //   }
  //   SECTION("Construct BNode(object).",
  //   "[Bencode][BNode][Constructor][Object]")
  //   {
  //     BNode bNode{ { "key1", 55 }, { "key2", 26666 } };
  //     REQUIRE_FALSE(!bNode.isObject());
  //     auto &object = BRef<Object>(bNode);
  //     REQUIRE(BRef<Number>(object["key1"]).value<int>() == 55);
  //     REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //   }
  //   SECTION("Construct BNode(array with nested array).",
  //   "[Bencode][BNode][Constructor][Array]")
  //   {
  //     BNode bNode{ 1, 2, 3, 4, BNode{ 5, 6, 7, 8 } };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //     REQUIRE(BRef<Number>(array[4][0]).value<int>() == 5);
  //     REQUIRE(BRef<Number>(array[4][1]).value<int>() == 6);
  //     REQUIRE(BRef<Number>(array[4][2]).value<int>() == 7);
  //     REQUIRE(BRef<Number>(array[4][3]).value<int>() == 8);
  //   }
  //   SECTION("Construct BNode(object with nested object).",
  //   "[Bencode][BNode][Constructor][Object]")
  //   {
  //     BNode bNode{ { "key1", 55 }, { "key2", 26666 }, { "key3", BNode{ {
  //     "key4", 5555 }, { "key5", 7777 } } } };
  //     REQUIRE_FALSE(!bNode.isObject());
  //     auto &object = BRef<Object>(bNode);
  //     REQUIRE(BRef<Number>(object["key1"]).value<int>() == 55);
  //     REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //     REQUIRE(BRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
  //     REQUIRE(BRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  //   }
  //   SECTION("Construct BNode(array with nested object).",
  //   "[Bencode][BNode][Constructor][Array]")
  //   {
  //     BNode bNode{ 1, 2, 3, 4, BNode{ { "key4", 5555 }, { "key5", 7777 } } };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //     REQUIRE(BRef<Number>(array[4]["key4"]).value<int>() == 5555);
  //     REQUIRE(BRef<Number>(array[4]["key5"]).value<int>() == 7777);
  //   }
  //   SECTION("Construct BNode(object with nested array).",
  //   "[Bencode][BNode][Constructor][Object]")
  //   {
  //     BNode bNode{ { "key1", 55 }, { "key2", 26666 }, { "key3", BNode{ 5, 6,
  //     7, 8 } } }; REQUIRE_FALSE(!bNode.isObject()); auto &object =
  //     BRef<Object>(bNode); REQUIRE(BRef<Number>(object["key1"]).value<int>()
  //     == 55); REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //     REQUIRE(BRef<Number>(object["key3"][0]).value<int>() == 5);
  //     REQUIRE(BRef<Number>(object["key3"][1]).value<int>() == 6);
  //     REQUIRE(BRef<Number>(object["key3"][2]).value<int>() == 7);
  //     REQUIRE(BRef<Number>(object["key3"][3]).value<int>() == 8);
  //   }
  // }
  // // =================
  // // BNode assignments
  // // =================
  // TEST_CASE("Check use of BNode assigment operators.",
  // "[Bencode][BNode][Assignment]")
  // {
  //   BNode bNode;
  //   SECTION("Assign integer to BNode.",
  //   "[Bencode][BNode][Assignment][Integer]")
  //   {
  //     bNode = 999;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<int>());
  //     REQUIRE(BRef<Number>(bNode).value<int>() == 999);
  //   }
  //   SECTION("Assign long to BNode.", "[Bencode][BNode][Assignment][Long]")
  //   {
  //     bNode = 99988899l;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<long>());
  //     REQUIRE(BRef<Number>(bNode).value<int>() == 99988899l);
  //   }
  //   SECTION("Assign long long to BNode.", "[Bencode][BNode][Assignment][Long
  //   Long]")
  //   {
  //     bNode = 99988899ll;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<long long>());
  //     REQUIRE(BRef<Number>(bNode).value<long long>() == 99988899ll);
  //   }
  //   SECTION("Assign float to BNode.", "[Bencode][BNode][Assignment][Float]")
  //   {
  //     bNode = 777.999f;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<float>());
  //     REQUIRE_FALSE(!equalFloatingPoint(BRef<Number>(bNode).value<float>(),
  //     777.999f, 0.0001));
  //   }
  //   SECTION("Assign double to BNode.",
  //   "[Bencode][BNode][Assignment][Double]")
  //   {
  //     bNode = 66666.8888;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<double>());
  //     REQUIRE_FALSE(!equalFloatingPoint(BRef<Number>(bNode).value<double>(),
  //     66666.8888, 0.0001));
  //   }
  //   SECTION("Assign long double to BNode.",
  //   "[Bencode][BNode][Assignment][Long Double]")
  //   {
  //     bNode = 66666.8888l;
  //     REQUIRE_FALSE(!bNode.isNumber());
  //     REQUIRE_FALSE(!BRef<Number>(bNode).is<long double>());
  //     REQUIRE_FALSE(!equalFloatingPoint(BRef<Number>(bNode).value<long
  //     double>(), 66666.8888l, 0.0001));
  //   }
  //   SECTION("Assign C string to BNode.",
  //   "[Bencode][BNode][Assignment][CString]")
  //   {
  //     bNode = "test string";
  //     REQUIRE_FALSE(!bNode.isString());
  //     REQUIRE(BRef<String>(bNode).value() == "test string");
  //   }
  //   SECTION("Assign string to BNode.",
  //   "[Bencode][BNode][Assignment][String]")
  //   {
  //     bNode = std::string("test string");
  //     REQUIRE_FALSE(!bNode.isString());
  //     REQUIRE(BRef<String>(bNode).value() == "test string");
  //   }
  //   SECTION("Assign boolean to BNode.",
  //   "[Bencode][BNode][Assignment][Boolean]")
  //   {
  //     bNode = false;
  //     REQUIRE_FALSE(!bNode.isBoolean());
  //     REQUIRE_FALSE(BRef<Boolean>(bNode).value());
  //   }
  //   SECTION("Assign nullptr to BNode.", "[Bencode][BNode][Assignment][Null]")
  //   {
  //     bNode = nullptr;
  //     REQUIRE_FALSE(!bNode.isNull());
  //     REQUIRE(BRef<Null>(bNode).getNull() == nullptr);
  //   }
  //   SECTION("Assign array to BNode.", "[Bencode][BNode][Assignment][Array]")
  //   {
  //     bNode = { 1, 2, 3, 4 };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //   }
  //   SECTION("Assign object to BNode.",
  //   "[Bencode][BNode][Assignment][Object]")
  //   {
  //     bNode = { { "key1", 55 }, { "key2", 26666 } };
  //     REQUIRE_FALSE(!bNode.isObject());
  //     auto &object = BRef<Object>(bNode);
  //     REQUIRE(BRef<Number>(object["key1"]).value<int>() == 55);
  //     REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //   }
  //   SECTION("Assign BNode with array with nested array.",
  //   "[Bencode][BNode][Constructor][Array]")
  //   {
  //     bNode = { 1, 2, 3, 4, BNode{ 5, 6, 7, 8 } };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //     REQUIRE(BRef<Number>(array[4][0]).value<int>() == 5);
  //     REQUIRE(BRef<Number>(array[4][1]).value<int>() == 6);
  //     REQUIRE(BRef<Number>(array[4][2]).value<int>() == 7);
  //     REQUIRE(BRef<Number>(array[4][3]).value<int>() == 8);
  //   }
  //   SECTION("Assign BNode with object with a nested object.",
  //   "[Bencode][BNode][Constructor][Object]")
  //   {
  //     bNode = { { "key1", 55 }, { "key2", 26666 }, { "key3", BNode{ { "key4",
  //     5555 }, { "key5", 7777 } } } }; REQUIRE_FALSE(!bNode.isObject()); auto
  //     &object = BRef<Object>(bNode);
  //     REQUIRE(BRef<Number>(object["key1"]).value<int>() == 55);
  //     REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //     REQUIRE(BRef<Number>(object["key3"]["key4"]).value<int>() == 5555);
  //     REQUIRE(BRef<Number>(object["key3"]["key5"]).value<int>() == 7777);
  //   }
  //   SECTION("Assign BNode with array with a nested object.",
  //   "[Bencode][BNode][Constructor][Array]")
  //   {
  //     bNode = { 1, 2, 3, 4, BNode{ { "key4", 5555 }, { "key5", 7777 } } };
  //     REQUIRE_FALSE(!bNode.isArray());
  //     auto &array = BRef<Array>(bNode).value();
  //     REQUIRE(BRef<Number>(array[0]).value<int>() == 1);
  //     REQUIRE(BRef<Number>(array[1]).value<int>() == 2);
  //     REQUIRE(BRef<Number>(array[2]).value<int>() == 3);
  //     REQUIRE(BRef<Number>(array[3]).value<int>() == 4);
  //     REQUIRE(BRef<Number>(array[4]["key4"]).value<int>() == 5555);
  //     REQUIRE(BRef<Number>(array[4]["key5"]).value<int>() == 7777);
  //   }
  //   SECTION("Assign BNode with object with nested a array.",
  //   "[Bencode][BNode][Constructor][Object]")
  //   {
  //     bNode = { { "key1", 55 }, { "key2", 26666 }, { "key3", BNode{ 5, 6, 7,
  //     8 } } }; REQUIRE_FALSE(!bNode.isObject()); auto &object =
  //     BRef<Object>(bNode); REQUIRE(BRef<Number>(object["key1"]).value<int>()
  //     == 55); REQUIRE(BRef<Number>(object["key2"]).value<int>() == 26666);
  //     REQUIRE(BRef<Number>(object["key3"][0]).value<int>() == 5);
  //     REQUIRE(BRef<Number>(object["key3"][1]).value<int>() == 6);
  //     REQUIRE(BRef<Number>(object["key3"][2]).value<int>() == 7);
  //     REQUIRE(BRef<Number>(object["key3"][3]).value<int>() == 8);
  //   }
}
// ==============
// BNode Indexing
// ==============
TEST_CASE("Use of BNode indexing operators", "[Bencode][BNode][Index]") {
  const Bencode bEncode;
  SECTION("Decode dictionary and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())["three"]).value() == "qwerty");
  }
  SECTION("Decode list and check its components using indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]).value() == "poiuytrewqas");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode list with embedded dictionary and check its components using "
          "indexing",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{"l6:sillyyd3:one10:01234567895:three6:qwerty3:"
                               "two9:asdfghjkle26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())[0]).value() == "sillyy");
    REQUIRE(BRef<String>((bEncode.root())[1]["one"]).value() == "0123456789");
    REQUIRE(BRef<String>((bEncode.root())[1]["two"]).value() == "asdfghjkl");
    REQUIRE(BRef<String>((bEncode.root())[1]["three"]).value() == "qwerty");
    REQUIRE(BRef<String>((bEncode.root())[2]).value() ==
            "abcdefghijklmnopqrstuvwxyz");
  }
  SECTION("Decode dictionary and check an invalid key generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())["onee"].isDictionary(), BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())["onee"].isDictionary(),
                        "BNode Error: Invalid key used in dictionary.");
  }
  SECTION("Decode list and check an invalid index generates exception",
          "[Bencode][BNode][Index]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS((bEncode.root())[3].isList(), BNode::Error);
    REQUIRE_THROWS_WITH((bEncode.root())[3].isList(),
                        "BNode Error: Invalid index used in list.");
  }
}
TEST_CASE("Check BNode reference functions work.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Integer reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"i45500e"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("String reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"10:0123456789"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<String>((bEncode.root())).value() == "0123456789");
  }
  SECTION("List reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<List>((bEncode.root())).size() == 3);
  }
  SECTION("Dictionary reference.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{
        "d4:four10:01234567893:one8:123456785:three6:qwerty3:two9:asdfghjkle"};
    bEncode.decode(bEncodeSource);
    REQUIRE(BRef<Dictionary>((bEncode.root())).size() == 4);
  }
  SECTION("BRef check node type correct.", "[Bencode][BNode][Reference]") {
    BufferSource bEncodeSource{"i45500e"};
    bEncode.decode(bEncodeSource);
    REQUIRE_THROWS_AS(BRef<Dictionary>(bEncode.root()), BNode::Error);
    REQUIRE_THROWS_WITH(BRef<Dictionary>(bEncode.root()),
                        "BNode Error: Node not a dictionary.");
  }
}