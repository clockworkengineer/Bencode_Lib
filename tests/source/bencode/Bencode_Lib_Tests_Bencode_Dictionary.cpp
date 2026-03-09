#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check Bencode dictionary creation api.",
          "[Bencode][Create][Dictionary]") {
  SECTION("Initialise Bencode with dictionary passed to string constructor.",
          "[Bencode][Create][Constructor]") {
    REQUIRE_NOTHROW(Bencode(R"(d2:pii3ee)"));
  }
  SECTION("Initialise Bencode with dictionary passed to string constructor and "
          "validate.",
          "[Bencode][Create][Constructor][Validate]") {
    const Bencode bencode(R"(d2:pii3ee)");
    REQUIRE_FALSE(!isA<Dictionary>(bencode.root()));
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(NRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Create dictionary with one entry containing an integer from an int.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 300;
    REQUIRE_FALSE(!isA<Integer>(bencode["integer"]));
    REQUIRE(NRef<Integer>(bencode["integer"]).value() == 300);
  }
  SECTION("Create dictionary with one entry containing "
          "an integer from a long"
          "from a long.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 30000l;
    REQUIRE_FALSE(!isA<Integer>(bencode["integer"]));
    REQUIRE(NRef<Integer>(bencode["integer"]).value() == 30000);
  }
  SECTION("Create dictionary with one entry containing "
          "an integer from a float).",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141f;
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(NRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Create dictionary with one entry containing "
          "an integer from a double.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(NRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Create dictionary with one entry containing "
          "a string from a const *char",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = "robert";
    REQUIRE_FALSE(!isA<String>(bencode["name"]));
    REQUIRE(NRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Create dictionary with one entry containing "
          "a string from a std::string.",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = std::string{"robert"};
    REQUIRE_FALSE(!isA<String>(bencode["name"]));
    REQUIRE(NRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Create dictionary with one entry containing "
          "an integer from a boolean.",
          "[Bencode][Create][Dictionary][Boolean]") {
    Bencode bencode;
    bencode["flag"] = true;
    REQUIRE_FALSE(!isA<Integer>(bencode["flag"]));
    REQUIRE(NRef<Integer>(bencode["flag"]).value() == 1);
  }
  SECTION("Create dictionary with one entry containing "
          "a integer from a nullptr.",
          "[Bencode][Create][Dictionary][null]") {
    Bencode bencode;
    bencode["nothing"] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]));
    REQUIRE(NRef<Integer>(bencode["nothing"]).value() == 0);
  }
  SECTION("Create two level dictionary and with an integer at base.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"] = 3;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]["extra"]));
    REQUIRE(NRef<Integer>(bencode["nothing"]["extra"]).value() == 3);
  }
  SECTION("Create three level dictionary and null at base and stringify.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]["extra"]["more"]));
    REQUIRE(NRef<Integer>(bencode["nothing"]["extra"]["more"]).value() == 0);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d7:nothingd5:extrad4:morei0eeee)");
  }
  SECTION("Create dictionary with multiple entries and verify root is a "
          "dictionary.",
          "[Bencode][Create][Dictionary][Types]") {
    Bencode bencode;
    bencode["a"] = 1;
    bencode["b"] = "hello";
    bencode["c"] = true;
    REQUIRE(isA<Dictionary>(bencode.root()));
    REQUIRE(isA<Integer>(bencode["a"]));
    REQUIRE(isA<String>(bencode["b"]));
    REQUIRE(isA<Integer>(bencode["c"]));
  }
  SECTION(
      "Create dictionary with a list value via initializer list and stringify.",
      "[Bencode][Create][Dictionary][List]") {
    Bencode bencode;
    bencode["nums"] = {10, 20, 30};
    REQUIRE(isA<List>(bencode["nums"]));
    REQUIRE(NRef<Integer>(bencode["nums"][0]).value() == 10);
    REQUIRE(NRef<Integer>(bencode["nums"][1]).value() == 20);
    REQUIRE(NRef<Integer>(bencode["nums"][2]).value() == 30);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d4:numsli10ei20ei30eee)");
  }
  SECTION("Create dictionary with a nested dictionary value via initializer "
          "list and stringify.",
          "[Bencode][Create][Dictionary][Nested]") {
    Bencode bencode;
    bencode["inner"] = {{"x", 1}, {"y", 2}};
    REQUIRE(isA<Dictionary>(bencode["inner"]));
    REQUIRE(NRef<Integer>(bencode["inner"]["x"]).value() == 1);
    REQUIRE(NRef<Integer>(bencode["inner"]["y"]).value() == 2);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d5:innerd1:xi1e1:yi2eee)");
  }
  SECTION("Overwrite an existing key's value and stringify.",
          "[Bencode][Create][Dictionary][Overwrite]") {
    Bencode bencode;
    bencode["key"] = 100;
    bencode["key"] = 200;
    REQUIRE(NRef<Integer>(bencode["key"]).value() == 200);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d3:keyi200ee)");
  }
  SECTION("Overwrite a key's value from integer to string and stringify.",
          "[Bencode][Create][Dictionary][Overwrite]") {
    Bencode bencode;
    bencode["key"] = 42;
    bencode["key"] = "changed";
    REQUIRE(isA<String>(bencode["key"]));
    REQUIRE(NRef<String>(bencode["key"]).value() == "changed");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d3:key7:changede)");
  }
  SECTION("Dictionary keys are sorted lexicographically in encoded output.",
          "[Bencode][Create][Dictionary][Ordering]") {
    Bencode bencode;
    bencode["zebra"] = 1;
    bencode["apple"] = 2;
    bencode["mango"] = 3;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d5:applei2e5:mangoi3e5:zebrai1ee)");
  }
  SECTION("Empty string value is stored and retrieved correctly.",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["empty"] = "";
    REQUIRE(isA<String>(bencode["empty"]));
    REQUIRE(NRef<String>(bencode["empty"]).value().empty());
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d5:empty0:e)");
  }
  SECTION("Large integer value is stored and retrieved correctly.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["big"] = 9999999999ll;
    REQUIRE(isA<Integer>(bencode["big"]));
    REQUIRE(NRef<Integer>(bencode["big"]).value() == 9999999999ll);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d3:bigi9999999999ee)");
  }
  SECTION("Negative integer value is stored and retrieved correctly.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["neg"] = -42;
    REQUIRE(isA<Integer>(bencode["neg"]));
    REQUIRE(NRef<Integer>(bencode["neg"]).value() == -42);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d3:negi-42ee)");
  }
  SECTION("Boolean false maps to integer zero.",
          "[Bencode][Create][Dictionary][Boolean]") {
    Bencode bencode;
    bencode["flag"] = false;
    REQUIRE(isA<Integer>(bencode["flag"]));
    REQUIRE(NRef<Integer>(bencode["flag"]).value() == 0);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d4:flagi0ee)");
  }
  SECTION("Round-trip: build dictionary, stringify, re-parse and verify.",
          "[Bencode][Create][Dictionary][RoundTrip]") {
    Bencode bencode;
    bencode["name"] = "Alice";
    bencode["score"] = 100;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(BufferSource{destination.toString()}));
    REQUIRE(isA<Dictionary>(bencode2.root()));
    REQUIRE(NRef<String>(bencode2["name"]).value() == "Alice");
    REQUIRE(NRef<Integer>(bencode2["score"]).value() == 100);
  }
}