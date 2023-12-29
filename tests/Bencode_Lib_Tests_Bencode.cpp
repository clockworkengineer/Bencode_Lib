//
// Unit Tests: Bencode_Lib_Tests_Bencode
//
// Description: Bencode creation API(s) unit tests for Bencode class
// using the Catch2 test framework. Note: These tests use more
// modern raw literal strings to represent Bencode.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

// ===========================
// Parse simple Bencode types
// ===========================
TEST_CASE("Check Bencode dictionary creation api.",
          "[Bencode][Create][Dictionary]") {
  SECTION("Initialise Bencode with Bencode dictionary passed to constructor.",
          "[Bencode][Create][Constructor]") {
    REQUIRE_NOTHROW(Bencode(R"(d2:pii3ee)"));
  }
  SECTION("Initialise Bencode with Bencode dictionary passed to constructor "
          "and validate.",
          "[Bencode][Create][Constructor][Validate]") {
    const Bencode bencode(R"(d2:pii3ee)");
    REQUIRE_FALSE(!bencode.root().isDictionary());
    REQUIRE_FALSE(!bencode["pi"].isInteger());
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from an int.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 300;
    REQUIRE_FALSE(!bencode["integer"].isInteger());
    REQUIRE(BRef<Integer>(bencode["integer"]).value() == 300);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a long"
          "from a long.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 30000l;
    REQUIRE_FALSE(!bencode["integer"].isInteger());
    REQUIRE(BRef<Integer>(bencode["integer"]).value() == 30000);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a float).",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141f;
    REQUIRE_FALSE(!bencode["pi"].isInteger());
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a double.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    REQUIRE_FALSE(!bencode["pi"].isInteger());
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a string from a const *char",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = "robert";
    REQUIRE_FALSE(!bencode["name"].isString());
    REQUIRE(BRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a string from a std::string.",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = std::string{"robert"};
    REQUIRE_FALSE(!bencode["name"].isString());
    REQUIRE(BRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a boolean.",
          "[Bencode][Create][Dictionary][Boolean]") {
    Bencode bencode;
    bencode["flag"] = true;
    REQUIRE_FALSE(!bencode["flag"].isInteger());
    REQUIRE(BRef<Integer>(bencode["flag"]).value() == 1);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a integer from a nullptr.",
          "[Bencode][Create][Dictionary][null]") {
    Bencode bencode;
    bencode["nothing"] = nullptr;
    REQUIRE_FALSE(!bencode["nothing"].isInteger());
    REQUIRE(BRef<Integer>(bencode["nothing"]).value() == 0);
  }
  SECTION("Create two level dictionary and null at base.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"] = 3;
    REQUIRE_FALSE(!bencode["nothing"]["extra"].isInteger());
    REQUIRE(BRef<Integer>(bencode["nothing"]["extra"]).value() == 3);
  }
  SECTION("Create three level dictionary and null at base and encode.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!bencode["nothing"]["extra"]["more"].isInteger());
    REQUIRE(BRef<Integer>(bencode["nothing"]["extra"]["more"]).value() == 0);
    BufferDestination destinationBuffer;
    REQUIRE_NOTHROW(bencode.encode(destinationBuffer));
    std::string result;
    for (auto ch : destinationBuffer.getBuffer())
      result.push_back(static_cast<char>(ch));
    REQUIRE(result == R"(d7:nothingd5:extrad4:morei0eeee)");
  }
}
TEST_CASE("Check Bencode list creation api.", "[Bencode][Create][List]") {
  SECTION("Initialise Bencode with Bencode list  passed to constructor.",
          "[Bencode][Create][Constructor]") {
    REQUIRE_NOTHROW(Bencode(R"(l2:pii3ee)"));
  }

  SECTION("Initialise Bencode with  list passed to constructor and validate.",
          "[Bencode][Create][Constructor][Validate]") {
    const Bencode bencode(R"(l2:pii3ee)");
    REQUIRE_FALSE(!bencode.root().isList());
    REQUIRE_FALSE(!bencode[0].isString());
    REQUIRE_FALSE(!bencode[1].isInteger());
    REQUIRE(BRef<String>(bencode[0]).value() == "pi");
    REQUIRE(BRef<Integer>(bencode[1]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a integer.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 300;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 300);
  }

  SECTION("Initialise root Bencode list with one entry containing a long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000l;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a long long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000ll;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a float.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141f;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a double.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a long double.",
      "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141l;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION(
      "Initialise root Bencode list with one entry containing a const char *.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = "robert";
    REQUIRE_FALSE(!bencode[0].isString());
    REQUIRE(BRef<String>(bencode[0]).value() == "robert");
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a std::string.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = std::string{"robert"};
    REQUIRE_FALSE(!bencode[0].isString());
    REQUIRE(BRef<String>(bencode[0]).value() == "robert");
  }
  SECTION("Initialise root Bencode list with one entry containing a boolean.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = true;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 1);
  }
  SECTION("Initialise root Bencode list with one entry containing a nullptr.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = nullptr;
    REQUIRE_FALSE(!bencode[0].isInteger());
    REQUIRE(BRef<Integer>(bencode[0]).value() == 0);
  }
}
//   SECTION("Create two level list with null at the base and stringify.",
//           "[Bencode][Create][List][null]") {
//     Bencode bencode;
//     bencode[0][0] = nullptr;
//     REQUIRE_FALSE(!bencode[0][0].isNull());
//     REQUIRE(BRef<Null>(bencode.root()[0][0]).getNull() == nullptr);
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() == R"([[null]])");
//   }
//   SECTION("Create list with free spaces string at the base and
//   stringify.",
//           "[Bencode][Create][List][null]") {
//     Bencode bencode;
//     bencode[5] = "test";
//     REQUIRE_FALSE(!bencode[5].isString());
//     REQUIRE(BRef<String>(bencode.root()[5]).value() == "test");
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() ==
//             R"([null,null,null,null,null,"test"])");
//   }
//   SECTION(
//       "Create list with free spaces add an number at the base and
//       stringify.",
//       "[Bencode][Create][List][null]") {
//     Bencode bencode;
//     bencode[5] = "test";
//     REQUIRE_FALSE(!bencode[5].isString());
//     REQUIRE(BRef<String>(bencode.root()[5]).value() == "test");
//     bencode[3] = 15;
//     REQUIRE_FALSE(!bencode[3].isInteger());
//     REQUIRE(BRef<Integer>(bencode.root()[3]).value<int>() == 15);
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() ==
//             R"([null,null,null,15,null,"test"])");
//   }
//   SECTION("Create list with initializer list assignment.",
//           "[Bencode][Create][List][initializer]") {
//     Bencode bencode;
//     bencode[5] = {1.0,   2.0,    3, 4.333, "5.0", "test test test test",
//                false, nullptr};
//     REQUIRE_FALSE(!bencode[5][0].isInteger());
//     REQUIRE_FALSE(!bencode[5][1].isInteger());
//     REQUIRE_FALSE(!bencode[5][2].isInteger());
//     REQUIRE_FALSE(!bencode[5][3].isInteger());
//     REQUIRE_FALSE(!bencode[5][4].isString());
//     REQUIRE_FALSE(!bencode[5][5].isString());
//     REQUIRE_FALSE(!bencode[5][6].isBoolean());
//     REQUIRE_FALSE(!bencode[5][7].isNull());
//     REQUIRE(BRef<Array>(bencode[5]).size() == 8);
//     REQUIRE_FALSE(!equalFloatingPoint(BRef<Integer>(bencode[5][0]).value<double>(),
//                                       1.0, 0.0001));
//     REQUIRE_FALSE(!equalFloatingPoint(BRef<Integer>(bencode[5][1]).value<double>(),
//                                       2.0, 0.0001));
//     REQUIRE(BRef<Integer>(bencode[5][2]).value<int>() == 3);
//     REQUIRE_FALSE(!equalFloatingPoint(BRef<Integer>(bencode[5][3]).value<double>(),
//                                       4.333, 0.0001));
//     REQUIRE(BRef<String>(bencode[5][4]).value() == "5.0");
//     REQUIRE(BRef<String>(bencode[5][5]).value() == "test test test test");
//     REQUIRE_FALSE(BRef<Boolean>(bencode[5][6]).value());
//     REQUIRE(BRef<Null>(bencode[5][7]).getNull() == nullptr);
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"([null,null,null,null,null,[1.0,2.0,3,4.333,"5.0","test test test
//         test",false,null]])");
//   }
// }
// TEST_CASE("Check Bencode create complex Bencode structures.",
//           "[Bencode][Create][Complex]") {
//   SECTION("A Single level dictionary.", "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"] = "Niels";
//     bencode["nothing"] = nullptr;
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() ==
//             R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null})");
//   }
//   SECTION("A two level dictionary.", "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"] = "Niels";
//     bencode["nothing"] = nullptr;
//     bencode["answer"]["everything"] = 42;
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42}})");
//   }
//   SECTION("A three level dictionary.", "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"][5] = "Niels";
//     bencode["nothing"] = nullptr;
//     bencode["answer"]["everything"][5] = 42;
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":[null,null,null,null,null,"Niels"],"nothing":null,"answer":{"everything":[null,null,null,null,null,42]}})");
//   }
//   SECTION("Object with sub list/dictionary create using initializer
//   list.",
//           "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"] = "Niels";
//     bencode["nothing"] = nullptr;
//     bencode["answer"]["everything"] = 42;
//     bencode["list"] = {1, 0, 2};
//     bencode["dictionary"] = {{"currency", "USD"}, {"value", 42.99}};
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"dictionary":{"currency":"USD","value":42.99}})");
//   }
//   SECTION("Object with sub list/dictionary with an embedded list create
//   using "
//           "initializer list.",
//           "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"] = "Niels";
//     bencode["nothing"] = nullptr;
//     bencode["answer"]["everything"] = 42;
//     bencode["list"] = {1, 0, 2};
//     bencode["dictionary"] = {{"currency", "USD"}, {"value", BNode{1, 2, 3,
//     4, 5}}}; BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"dictionary":{"currency":"USD","value":[1,2,3,4,5]}})");
//   }
//   SECTION("Object with sub list/dictionary with an embedded dictionary
//   create using "
//           "initializer list.",
//           "[Bencode][Create][Complex]") {
//     Bencode bencode;
//     bencode["pi"] = 3.141;
//     bencode["happy"] = true;
//     bencode["name"] = "Niels";
//     bencode["nothing"] = nullptr;
//     bencode["answer"]["everything"] = 42;
//     bencode["list"] = {1, 0, 2};
//     bencode["dictionary"] = {{"currency", "USD"},
//                       {"value", BNode{{"key1", 22}, {"key2", 99.899}}}};
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"dictionary":{"currency":"USD","value":{"key1":22,"key2":99.899}}})");
//   }
//   SECTION("Array creation completely using a initializer list.",
//           "[Bencode][Create][Complex][Initializer") {
//     Bencode bencode = {1, 2, 3, 4};
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() == R"([1,2,3,4])");
//   }
//   SECTION("Object creation completely using a initializer list.",
//           "[Bencode][Create][Complex][Initializer") {
//     Bencode bencode = {{"currency", "USD"}, {"value", 42.99}};
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(bencodeDestination.getBuffer() ==
//             R"({"currency":"USD","value":42.99})");
//   }
//   SECTION("Object creation completely using a nested initializer list.",
//           "[Bencode][Create][Complex][Initializer") {
//     // Note: For the moment has to explicitly uses BNode to create a
//     // nested dictionary/list
//     Bencode bencode = {{"pi", 3.141},
//                     {"happy", true},
//                     {"name", "Niels"},
//                     {"nothing", nullptr},
//                     {"answer", BNode{{"everything", 42}}},
//                     {"list", BNode{1, 0, 2}},
//                     {"dictionary", BNode{{"currency", "USD"},
//                     {"value", 42.99}}}};
//     BufferDestination bencodeDestination;
//     REQUIRE_NOTHROW(bencode.stringify(bencodeDestination));
//     REQUIRE(
//         bencodeDestination.getBuffer() ==
//         R"({"pi":3.141,"happy":true,"name":"Niels","nothing":null,"answer":{"everything":42},"list":[1,0,2],"dictionary":{"currency":"USD","value":42.99}})");
//   }
// }