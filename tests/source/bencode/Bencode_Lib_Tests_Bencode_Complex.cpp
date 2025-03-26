#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check Bencode create complex Bencode structures.",
          "[Bencode][Create][Complex]") {
  SECTION("A Single level dictionary.", "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"] = "Niels";
    bencode["nothing"] = nullptr;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() ==
            R"(d5:happyi1e4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("A two level dictionary.", "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"] = "Niels";
    bencode["nothing"] = nullptr;
    bencode["answer"]["everything"] = 42;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee5:happyi1e4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("A three level dictionary.", "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"][5] = "Niels";
    bencode["nothing"] = nullptr;
    bencode["answer"]["everything"]["last"] = 42;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingd4:lasti42eee5:happyi1e4:namel5:Nielse7:nothingi0e2:pii3ee)");
  }
  SECTION("Dictionary with sub list/dictionary created using initializer list.",
          "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"] = "Niels";
    bencode["nothing"] = nullptr;
    bencode["answer"]["everything"] = 42;
    bencode["list"] = {1, 0, 2};
    bencode["dictionary"] = {{"currency", "USD"}, {"value", 42.99}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("Dictionary with sub list/dictionary with an embedded list created using "
          "initializer list.",
          "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"] = "Niels";
    bencode["nothing"] = nullptr;
    bencode["answer"]["everything"] = 42;
    bencode["list"] = {1, 0, 2};
    bencode["dictionary"] = {{"currency", "USD"},
                             {"value", BNode{1, 2, 3, 4, 5}}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valueli1ei2ei3ei4ei5eee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("Object with sub list/dictionary with an embedded dictionary created "
          "using initializer list.",
          "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["pi"] = 3.141;
    bencode["happy"] = true;
    bencode["name"] = "Niels";
    bencode["nothing"] = nullptr;
    bencode["answer"]["everything"] = 42;
    bencode["list"] = {1, 0, 2};
    bencode["dictionary"] = {{"currency", "USD"},
                             {"value", BNode{{"key1", 22}, {"key2", 99.899}}}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valued4:key1i22e4:key2i99eee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("List creation completely using a initializer list assignment.",
          "[Bencode][Create][Complex][Initializer]") {
    Bencode bencode = {1, 2, 3, 4};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li1ei2ei3ei4ee)");
  }
  SECTION("Dictionary creation completely using a initializer list assignment.",
          "[Bencode][Create][Complex][Initializer]") {
    Bencode bencode = {{"currency", "USD"}, {"value", 42.99}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d8:currency3:USD5:valuei42ee)");
  }
  SECTION(
      "Dictionary creation completely using a nested initializer list assignment.",
      "[Bencode][Create][Complex][Initializer]") {
    // Note: For the moment has to explicitly uses BNode to create a
    // nested dictionary/list
    Bencode bencode = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", BNode{{"everything", 42}}},
        {"list", BNode{1, 0, 2}},
        {"dictionary", BNode{{"currency", "USD"}, {"value", 42.99}}}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }

  SECTION("List creation completely using an initializer list.",
          "[Bencode][Create][Complex][Initializer]") {
    Bencode bencode{1, 2, 3, 4};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li1ei2ei3ei4ee)");
  }
  SECTION("Object creation completely using an initializer list.",
          "[Bencode][Create][Complex][Initializer]") {
    Bencode bencode{{"currency", "USD"}, {"value", 42.99}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d8:currency3:USD5:valuei42ee)");
  }
  SECTION("Object creation completely using an nested initializer list.",
          "[Bencode][Create][Complex][Initializer]") {
    // Note: For the moment has to explicitly uses BNode to create a
    // nested dictionary/list
    Bencode bencode{
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", BNode{{"everything", 42}}},
        {"list", BNode{1, 0, 2}},
        {"dictionary", BNode{{"currency", "USD"}, {"value", 42.99}}}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }
}