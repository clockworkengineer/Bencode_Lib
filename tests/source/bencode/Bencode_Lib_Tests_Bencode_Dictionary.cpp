#include "Bencode_Lib_Tests.hpp"

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
    REQUIRE_FALSE(!isA<Dictionary>(bencode.root()));
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from an int.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 300;
    REQUIRE_FALSE(!isA<Integer>(bencode["integer"]));
    REQUIRE(BRef<Integer>(bencode["integer"]).value() == 300);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a long"
          "from a long.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["integer"] = 30000l;
    REQUIRE_FALSE(!isA<Integer>(bencode["integer"]));
    REQUIRE(BRef<Integer>(bencode["integer"]).value() == 30000);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a float).",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141f;
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a double.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    REQUIRE_FALSE(!isA<Integer>(bencode["pi"]));
    REQUIRE(BRef<Integer>(bencode["pi"]).value() == 3);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a string from a const *char",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = "robert";
    REQUIRE_FALSE(!isA<String>(bencode["name"]));
    REQUIRE(BRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a string from a std::string.",
          "[Bencode][Create][Dictionary][String]") {
    Bencode bencode;
    bencode["name"] = std::string{"robert"};
    REQUIRE_FALSE(!isA<String>(bencode["name"]));
    REQUIRE(BRef<String>(bencode["name"]).value() == "robert");
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "an integer from a boolean.",
          "[Bencode][Create][Dictionary][Boolean]") {
    Bencode bencode;
    bencode["flag"] = true;
    REQUIRE_FALSE(!isA<Integer>(bencode["flag"]));
    REQUIRE(BRef<Integer>(bencode["flag"]).value() == 1);
  }
  SECTION("Initialise root Bencode BNode dictionary with one entry containing "
          "a integer from a nullptr.",
          "[Bencode][Create][Dictionary][null]") {
    Bencode bencode;
    bencode["nothing"] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]));
    REQUIRE(BRef<Integer>(bencode["nothing"]).value() == 0);
  }
  SECTION("Create two level dictionary and null at base.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"] = 3;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]["extra"]));
    REQUIRE(BRef<Integer>(bencode["nothing"]["extra"]).value() == 3);
  }
  SECTION("Create three level dictionary and null at base and encode.",
          "[Bencode][Create][Dictionary][Integer]") {
    Bencode bencode;
    bencode["nothing"]["extra"]["more"] = nullptr;
    REQUIRE_FALSE(!isA<Integer>(bencode["nothing"]["extra"]["more"]));
    REQUIRE(BRef<Integer>(bencode["nothing"]["extra"]["more"]).value() == 0);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.encode(destination));
    REQUIRE(destination.toString() == R"(d7:nothingd5:extrad4:morei0eeee)");
  }
}