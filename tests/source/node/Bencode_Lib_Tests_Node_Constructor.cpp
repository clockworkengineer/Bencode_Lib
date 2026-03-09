#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check use of Node constructors.", "[Bencode][Node][Constructor]") {
  SECTION("Construct Node(integer).", "[Bencode][Node][Constructor][Integer]") {
    Node bNode(999);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 999);
  }
  SECTION("Construct Node(long).", "[Bencode][Node][Constructor][Long]") {
    Node bNode(99988899l);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 99988899l);
  }
  SECTION("Construct Node(long long).",
          "[Bencode][Node][Constructor][Long Long]") {
    Node bNode(99988899ll);
    REQUIRE_FALSE(!isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 99988899ll);
  }

  SECTION("Construct Node(C string).",
          "[Bencode][Node][Constructor][CString]") {
    Node bNode("test string");
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(NRef<String>(bNode).value() == "test string");
  }
  SECTION("Construct Node(string).", "[Bencode][Node][Constructor][String]") {
    std::string string{"test string"};
    Node bNode(string);
    REQUIRE_FALSE(!isA<String>(bNode));
    REQUIRE(NRef<String>(bNode).value() == "test string");
  }
  SECTION("Construct Node(list).", "[Bencode][Node][Constructor][List]") {
    Node bNode{1, 2, 3, 4};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(list[0]).value() == 1);
    REQUIRE(NRef<Integer>(list[1]).value() == 2);
    REQUIRE(NRef<Integer>(list[2]).value() == 3);
    REQUIRE(NRef<Integer>(list[3]).value() == 4);
  }
  SECTION("Construct Node(dictionary).",
          "[Bencode][Node][Constructor][Dictionary]") {
    Node bNode{{"key1", 55}, {"key2", 26666}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
  }
  SECTION("Construct Node(list with nested list).",
          "[Bencode][Node][Constructor][Array]") {
    Node bNode{1, 2, 3, 4, Node{5, 6, 7, 8}};
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
  SECTION("Construct Node(dictionary with nested dictionary).",
          "[Bencode][Node][Constructor][Dictionary]") {
    Node bNode{{"key1", 55},
               {"key2", 26666},
               {"key3", Node{{"key4", 5555}, {"key5", 7777}}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(NRef<Integer>(dictionary["key3"]["key4"]).value() == 5555);
    REQUIRE(NRef<Integer>(dictionary["key3"]["key5"]).value() == 7777);
  }
  SECTION("Construct Node(list with nested dictionary).",
          "[Bencode][Node][Constructor][List]") {
    Node bNode{1, 2, 3, 4, Node{{"key4", 5555}, {"key5", 7777}}};
    REQUIRE_FALSE(!isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(list[0]).value() == 1);
    REQUIRE(NRef<Integer>(list[1]).value() == 2);
    REQUIRE(NRef<Integer>(list[2]).value() == 3);
    REQUIRE(NRef<Integer>(list[3]).value() == 4);
    REQUIRE(NRef<Integer>(list[4]["key4"]).value() == 5555);
    REQUIRE(NRef<Integer>(list[4]["key5"]).value() == 7777);
  }
  SECTION("Construct Node(dictionary with nested list).",
          "[Bencode][Node][Constructor][Dictionary]") {
    Node bNode{{"key1", 55}, {"key2", 26666}, {"key3", Node{5, 6, 7, 8}}};
    REQUIRE_FALSE(!isA<Dictionary>(bNode));
    auto &dictionary = NRef<Dictionary>(bNode);
    REQUIRE(NRef<Integer>(dictionary["key1"]).value() == 55);
    REQUIRE(NRef<Integer>(dictionary["key2"]).value() == 26666);
    REQUIRE(NRef<Integer>(dictionary["key3"][0]).value() == 5);
    REQUIRE(NRef<Integer>(dictionary["key3"][1]).value() == 6);
    REQUIRE(NRef<Integer>(dictionary["key3"][2]).value() == 7);
    REQUIRE(NRef<Integer>(dictionary["key3"][3]).value() == 8);
  }
  SECTION("Construct Node(float) stores as truncated integer.",
          "[Bencode][Node][Constructor][Float]") {
    Node bNode(3.99f);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 3);
  }
  SECTION("Construct Node(double) stores as truncated integer.",
          "[Bencode][Node][Constructor][Double]") {
    Node bNode(9.99);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 9);
  }
  SECTION("Construct Node(negative integer).",
          "[Bencode][Node][Constructor][Integer]") {
    Node bNode(-100);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == -100);
  }
  SECTION("Construct Node(boolean true) stores as integer one.",
          "[Bencode][Node][Constructor][Boolean]") {
    Node bNode(true);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 1);
  }
  SECTION("Construct Node(boolean false) stores as integer zero.",
          "[Bencode][Node][Constructor][Boolean]") {
    Node bNode(false);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 0);
  }
  SECTION("Construct Node(nullptr) stores as integer zero.",
          "[Bencode][Node][Constructor][Null]") {
    Node bNode(nullptr);
    REQUIRE(isA<Integer>(bNode));
    REQUIRE(NRef<Integer>(bNode).value() == 0);
  }
  SECTION("Construct Node(empty C string).",
          "[Bencode][Node][Constructor][String]") {
    Node bNode("");
    REQUIRE(isA<String>(bNode));
    REQUIRE(NRef<String>(bNode).value().empty());
  }
  SECTION("Default constructed Node is empty.",
          "[Bencode][Node][Constructor]") {
    Node bNode;
    REQUIRE(bNode.isEmpty());
  }
  SECTION("Construct Node(list of strings).",
          "[Bencode][Node][Constructor][List]") {
    Node bNode{"one", "two", "three"};
    REQUIRE(isA<List>(bNode));
    auto &list = NRef<List>(bNode).value();
    REQUIRE(NRef<String>(list[0]).value() == "one");
    REQUIRE(NRef<String>(list[1]).value() == "two");
    REQUIRE(NRef<String>(list[2]).value() == "three");
  }
  SECTION("Construct Node(dictionary with string values).",
          "[Bencode][Node][Constructor][Dictionary]") {
    Node bNode{{"name", "Alice"}, {"role", "admin"}};
    REQUIRE(isA<Dictionary>(bNode));
    auto &dict = NRef<Dictionary>(bNode);
    REQUIRE(NRef<String>(dict["name"]).value() == "Alice");
    REQUIRE(NRef<String>(dict["role"]).value() == "admin");
  }
  SECTION("Construct Node(list of lists).",
          "[Bencode][Node][Constructor][List]") {
    Node bNode{Node{1, 2}, Node{3, 4}};
    REQUIRE(isA<List>(bNode));
    auto &outer = NRef<List>(bNode).value();
    REQUIRE(NRef<Integer>(outer[0][0]).value() == 1);
    REQUIRE(NRef<Integer>(outer[0][1]).value() == 2);
    REQUIRE(NRef<Integer>(outer[1][0]).value() == 3);
    REQUIRE(NRef<Integer>(outer[1][1]).value() == 4);
  }
  SECTION("Node constructed as list has correct size.",
          "[Bencode][Node][Constructor][List]") {
    Node bNode{10, 20, 30, 40, 50};
    REQUIRE(isA<List>(bNode));
    REQUIRE(NRef<List>(bNode).size() == 5);
  }
  SECTION("Node constructed as dictionary has correct entry count.",
          "[Bencode][Node][Constructor][Dictionary]") {
    Node bNode{{"a", 1}, {"b", 2}, {"c", 3}};
    REQUIRE(isA<Dictionary>(bNode));
    REQUIRE(NRef<Dictionary>(bNode).size() == 3);
  }
}