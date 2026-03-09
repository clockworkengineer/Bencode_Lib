#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check Bencode list creation api.", "[Bencode][Create][List]") {
  SECTION("Initialise Bencode with Bencode list  passed to string constructor.",
          "[Bencode][Create][Constructor]") {
    REQUIRE_NOTHROW(Bencode(R"(l2:pii3ee)"));
  }

  SECTION(
      "Initialise Bencode with list passed to string constructor and validate.",
      "[Bencode][Create][Constructor][Validate]") {
    const Bencode bencode(R"(l2:pii3ee)");
    REQUIRE_FALSE(!isA<List>(bencode.root()));
    REQUIRE_FALSE(!isA<String>(bencode[0]));
    REQUIRE_FALSE(!isA<Integer>(bencode[1]));
    REQUIRE(NRef<String>(bencode[0]).value() == "pi");
    REQUIRE(NRef<Integer>(bencode[1]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a integer.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 300;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 300);
  }

  SECTION("Initialise root Bencode list with one entry containing a long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000l;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a long long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000ll;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a float.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141f;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a double.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 3);
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a long double.",
      "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141l;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION(
      "Initialise root Bencode list with one entry containing a const char *.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = "robert";
    REQUIRE_FALSE(!isA<String>(bencode[0]));
    REQUIRE(NRef<String>(bencode[0]).value() == "robert");
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a std::string.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = std::string{"robert"};
    REQUIRE_FALSE(!isA<String>(bencode[0]));
    REQUIRE(NRef<String>(bencode[0]).value() == "robert");
  }
  SECTION("Initialise root Bencode list with one entry containing a boolean.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = true;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 1);
  }
  SECTION("Initialise root Bencode list with one entry containing a nullptr.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 0);
  }
  SECTION("Create two level list with an integer at the base and stringify it.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0][0] = 3000;
    REQUIRE_FALSE(!isA<Integer>(bencode[0][0]));
    REQUIRE(NRef<Integer>(bencode[0][0]).value() == 3000);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(lli3000eee)");
  }
  SECTION(
      "Create list with free spaces and a string at the base and stringify.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[5] = "test";
    REQUIRE_FALSE(!isA<String>(bencode[5]));
    REQUIRE(NRef<String>(bencode[5]).value() == "test");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(l4:teste)");
  }
  SECTION("Create list with 5 free spaces, add an string to the last entry, "
          "add an integer at element 3 and stringify.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[5] = "test";
    REQUIRE_FALSE(!isA<String>(bencode[5]));
    REQUIRE(NRef<String>(bencode[5]).value() == "test");
    bencode[3] = 15;
    REQUIRE_FALSE(!isA<Integer>(bencode[3]));
    REQUIRE(NRef<Integer>(bencode[3]).value() == 15);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li15e4:teste)");
  }

  SECTION("Create list with initializer list assignment.",
          "[Bencode][Create][List][initializer]") {
    Bencode bencode;
    bencode[5] = {1.0,  2.0,    3, 4.333, "5.0", "test test test test",
                  true, nullptr};
    REQUIRE_FALSE(!isA<Integer>(bencode[5][0]));
    REQUIRE_FALSE(!isA<Integer>(bencode[5][1]));
    REQUIRE_FALSE(!isA<Integer>(bencode[5][2]));
    REQUIRE_FALSE(!isA<Integer>(bencode[5][3]));
    REQUIRE_FALSE(!isA<String>(bencode[5][4]));
    REQUIRE_FALSE(!isA<String>(bencode[5][5]));
    REQUIRE_FALSE(!isA<Integer>(bencode[5][6]));
    REQUIRE_FALSE(!isA<Integer>(bencode[5][7]));
    REQUIRE(NRef<List>(bencode[5]).size() == 8);
    REQUIRE(NRef<Integer>(bencode[5][0]).value() == 1);
    REQUIRE(NRef<Integer>(bencode[5][1]).value() == 2);
    REQUIRE(NRef<Integer>(bencode[5][2]).value() == 3);
    REQUIRE(NRef<String>(bencode[5][4]).value() == "5.0");
    REQUIRE(NRef<String>(bencode[5][5]).value() == "test test test test");
    REQUIRE(NRef<Integer>(bencode[5][6]).value() == 1);
    REQUIRE(NRef<Integer>(bencode[5][7]).value() == 0);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() ==
            R"(lli1ei2ei3ei4e3:5.019:test test test testi1ei0eee)");
  }
  SECTION("Verify root of a list built by index is a List.",
          "[Bencode][Create][List][Types]") {
    Bencode bencode;
    bencode[0] = 1;
    bencode[1] = 2;
    REQUIRE(isA<List>(bencode.root()));
  }
  SECTION("Multi-element list of integers stringifies correctly.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 10;
    bencode[1] = 20;
    bencode[2] = 30;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li10ei20ei30ee)");
  }
  SECTION("Multi-element list of strings stringifies correctly.",
          "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = "one";
    bencode[1] = "two";
    bencode[2] = "three";
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(l3:one3:two5:threee)");
  }
  SECTION("Overwrite an existing list element and verify new value.",
          "[Bencode][Create][List][Overwrite]") {
    Bencode bencode;
    bencode[0] = 99;
    bencode[0] = 42;
    REQUIRE(isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 42);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li42ee)");
  }
  SECTION("Overwrite a list element changing type from integer to string.",
          "[Bencode][Create][List][Overwrite]") {
    Bencode bencode;
    bencode[0] = 7;
    bencode[0] = "seven";
    REQUIRE(isA<String>(bencode[0]));
    REQUIRE(NRef<String>(bencode[0]).value() == "seven");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(l5:sevene)");
  }
  SECTION("Negative integer stored in a list.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = -100;
    REQUIRE(isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == -100);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li-100ee)");
  }
  SECTION("Empty string stored in a list.", "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = "";
    REQUIRE(isA<String>(bencode[0]));
    REQUIRE(NRef<String>(bencode[0]).value().empty());
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(l0:e)");
  }
  SECTION("Three level nested list stringifies correctly.",
          "[Bencode][Create][List][Nested]") {
    Bencode bencode;
    bencode[0][0][0] = 7;
    REQUIRE(isA<Integer>(bencode[0][0][0]));
    REQUIRE(NRef<Integer>(bencode[0][0][0]).value() == 7);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(llli7eeee)");
  }
  SECTION("List containing a nested dictionary via Node initializer list.",
          "[Bencode][Create][List][Nested]") {
    Bencode bencode;
    bencode[0] = Node{{"key", "val"}};
    REQUIRE(isA<Dictionary>(bencode[0]));
    REQUIRE(NRef<String>(bencode[0]["key"]).value() == "val");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(ld3:key3:valee)");
  }
  SECTION(
      "List created from initializer list assignment stringifies correctly.",
      "[Bencode][Create][List][Initializer]") {
    Bencode bencode;
    bencode[0] = {1, 2, 3};
    REQUIRE(isA<List>(bencode[0]));
    REQUIRE(NRef<List>(bencode[0]).size() == 3);
    REQUIRE(NRef<Integer>(bencode[0][0]).value() == 1);
    REQUIRE(NRef<Integer>(bencode[0][1]).value() == 2);
    REQUIRE(NRef<Integer>(bencode[0][2]).value() == 3);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(lli1ei2ei3eee)");
  }
  SECTION("Round-trip: build list, stringify, re-parse and verify.",
          "[Bencode][Create][List][RoundTrip]") {
    Bencode bencode;
    bencode[0] = 42;
    bencode[1] = "hello";
    bencode[2] = 0;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(BufferSource{destination.toString()}));
    REQUIRE(isA<List>(bencode2.root()));
    REQUIRE(NRef<Integer>(bencode2[0]).value() == 42);
    REQUIRE(NRef<String>(bencode2[1]).value() == "hello");
    REQUIRE(NRef<Integer>(bencode2[2]).value() == 0);
  }
}