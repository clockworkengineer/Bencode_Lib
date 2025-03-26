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
    REQUIRE(BRef<String>(bencode[0]).value() == "pi");
    REQUIRE(BRef<Integer>(bencode[1]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a integer.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 300;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 300);
  }

  SECTION("Initialise root Bencode list with one entry containing a long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000l;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a long long.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 30000ll;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 30000);
  }

  SECTION("Initialise root Bencode list with one entry containing a float.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141f;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION("Initialise root Bencode list with one entry containing a double.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a long double.",
      "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = 3.141l;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 3);
  }

  SECTION(
      "Initialise root Bencode list with one entry containing a const char *.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = "robert";
    REQUIRE_FALSE(!isA<String>(bencode[0]));
    REQUIRE(BRef<String>(bencode[0]).value() == "robert");
  }
  SECTION(
      "Initialise root Bencode list with one entry containing a std::string.",
      "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[0] = std::string{"robert"};
    REQUIRE_FALSE(!isA<String>(bencode[0]));
    REQUIRE(BRef<String>(bencode[0]).value() == "robert");
  }
  SECTION("Initialise root Bencode list with one entry containing a boolean.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = true;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 1);
  }
  SECTION("Initialise root Bencode list with one entry containing a nullptr.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode[0]));
    REQUIRE(BRef<Integer>(bencode[0]).value() == 0);
  }
  SECTION("Create two level list with an integer at the base and stringify it.",
          "[Bencode][Create][List][Integer]") {
    Bencode bencode;
    bencode[0][0] = 3000;
    REQUIRE_FALSE(!isA<Integer>(bencode[0][0]));
    REQUIRE(BRef<Integer>(bencode[0][0]).value() == 3000);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(lli3000eee)");
  }
  SECTION("Create list with free spaces and a string at the base and stringify.",
          "[Bencode][Create][List][String]") {
    Bencode bencode;
    bencode[5] = "test";
    REQUIRE_FALSE(!isA<String>(bencode[5]));
    REQUIRE(BRef<String>(bencode[5]).value() == "test");
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
    REQUIRE(BRef<String>(bencode[5]).value() == "test");
    bencode[3] = 15;
    REQUIRE_FALSE(!isA<Integer>(bencode[3]));
    REQUIRE(BRef<Integer>(bencode[3]).value() == 15);
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
    REQUIRE(BRef<List>(bencode[5]).size() == 8);
    REQUIRE(BRef<Integer>(bencode[5][0]).value() == 1);
    REQUIRE(BRef<Integer>(bencode[5][1]).value() == 2);
    REQUIRE(BRef<Integer>(bencode[5][2]).value() == 3);
    REQUIRE(BRef<String>(bencode[5][4]).value() == "5.0");
    REQUIRE(BRef<String>(bencode[5][5]).value() == "test test test test");
    REQUIRE(BRef<Integer>(bencode[5][6]).value() == 1);
    REQUIRE(BRef<Integer>(bencode[5][7]).value() == 0);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() ==
            R"(lli1ei2ei3ei4e3:5.019:test test test testi1ei0eee)");
  }
}