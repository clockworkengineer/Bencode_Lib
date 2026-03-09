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
  SECTION(
      "Dictionary with sub list/dictionary with an embedded list created using "
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
                             {"value", Node{1, 2, 3, 4, 5}}};
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
                             {"value", Node{{"key1", 22}, {"key2", 99.899}}}};
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
  SECTION("Dictionary creation completely using a nested initializer list "
          "assignment.",
          "[Bencode][Create][Complex][Initializer]") {
    // Note: For the moment has to explicitly uses Node to create a
    // nested dictionary/list
    Bencode bencode = {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", Node{{"everything", 42}}},
        {"list", Node{1, 0, 2}},
        {"dictionary", Node{{"currency", "USD"}, {"value", 42.99}}}};
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
    // Note: For the moment has to explicitly uses Node to create a
    // nested dictionary/list
    Bencode bencode{
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {"answer", Node{{"everything", 42}}},
        {"list", Node{1, 0, 2}},
        {"dictionary", Node{{"currency", "USD"}, {"value", 42.99}}}};
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(
        destination.toString() ==
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)");
  }
  SECTION("Round-trip: stringify a built structure then re-parse and verify "
          "values.",
          "[Bencode][Create][Complex][RoundTrip]") {
    Bencode bencode;
    bencode["name"] = "test";
    bencode["count"] = 99;
    bencode["active"] = true;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(BufferSource{destination.toString()}));
    REQUIRE(isA<String>(bencode2["name"]));
    REQUIRE(NRef<String>(bencode2["name"]).value() == "test");
    REQUIRE(isA<Integer>(bencode2["count"]));
    REQUIRE(NRef<Integer>(bencode2["count"]).value() == 99);
    REQUIRE(isA<Integer>(bencode2["active"]));
    REQUIRE(NRef<Integer>(bencode2["active"]).value() == 1);
  }
  SECTION("Round-trip: parse a known Bencode string then stringify matches "
          "original.",
          "[Bencode][Create][Complex][RoundTrip]") {
    const std::string encoded =
        R"(d6:answerd10:everythingi42ee4:listli1ei0ei2ee4:name5:Nielse)";
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{encoded}));
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == encoded);
  }
  SECTION("Overwriting an existing dictionary value updates it correctly.",
          "[Bencode][Create][Complex][Overwrite]") {
    Bencode bencode;
    bencode["x"] = 1;
    bencode["x"] = 2;
    REQUIRE(isA<Integer>(bencode["x"]));
    REQUIRE(NRef<Integer>(bencode["x"]).value() == 2);
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d1:xi2ee)");
  }
  SECTION("Overwriting a string value with another string.",
          "[Bencode][Create][Complex][Overwrite]") {
    Bencode bencode;
    bencode["label"] = "first";
    bencode["label"] = "second";
    REQUIRE(isA<String>(bencode["label"]));
    REQUIRE(NRef<String>(bencode["label"]).value() == "second");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d5:label6:seconde)");
  }
  SECTION("List of strings built via index operator.",
          "[Bencode][Create][Complex][List]") {
    Bencode bencode;
    bencode[0] = "alpha";
    bencode[1] = "beta";
    bencode[2] = "gamma";
    REQUIRE(isA<String>(bencode[0]));
    REQUIRE(isA<String>(bencode[1]));
    REQUIRE(isA<String>(bencode[2]));
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(l5:alpha4:beta5:gammae)");
  }
  SECTION("Mixed list of integers and strings built via index operator.",
          "[Bencode][Create][Complex][List]") {
    Bencode bencode;
    bencode[0] = 1;
    bencode[1] = "two";
    bencode[2] = 3;
    REQUIRE(isA<Integer>(bencode[0]));
    REQUIRE(isA<String>(bencode[1]));
    REQUIRE(isA<Integer>(bencode[2]));
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li1e3:twoi3ee)");
  }
  SECTION("Sequential list built in a loop stringifies correctly.",
          "[Bencode][Create][Complex][List]") {
    Bencode bencode;
    for (int i = 0; i < 5; i++) {
      bencode[i] = i + 1;
    }
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(li1ei2ei3ei4ei5ee)");
  }
  SECTION("Nested list inside a dictionary built via index and key operators.",
          "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["outer"]["inner"][0] = 10;
    bencode["outer"]["inner"][1] = 20;
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d5:outerd5:innerli10ei20eeee)");
  }
  SECTION("List of dictionaries built via Node initializer list.",
          "[Bencode][Create][Complex][Initializer]") {
    Bencode bencode;
    bencode[0] = Node{{"key", "val"}};
    bencode[1] = Node{{"num", 42}};
    REQUIRE(isA<Dictionary>(bencode[0]));
    REQUIRE(isA<Dictionary>(bencode[1]));
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(ld3:key3:valed3:numi42eee)");
  }
  SECTION("Type checks on a complex built structure.",
          "[Bencode][Create][Complex][Types]") {
    Bencode bencode;
    bencode["count"] = 5;
    bencode["items"] = {1, 2, 3, 4, 5};
    bencode["label"] = "hello";
    REQUIRE(isA<Dictionary>(bencode.root()));
    REQUIRE(isA<Integer>(bencode["count"]));
    REQUIRE(NRef<Integer>(bencode["count"]).value() == 5);
    REQUIRE(isA<List>(bencode["items"]));
    REQUIRE(isA<String>(bencode["label"]));
    REQUIRE(NRef<String>(bencode["label"]).value() == "hello");
  }
  SECTION("Deeply nested three-level list inside a dictionary.",
          "[Bencode][Create][Complex]") {
    Bencode bencode;
    bencode["a"]["b"][0] = "x";
    bencode["a"]["b"][1] = "y";
    bencode["a"]["b"][2] = "z";
    REQUIRE(isA<Dictionary>(bencode.root()));
    REQUIRE(isA<Dictionary>(bencode["a"]));
    REQUIRE(isA<List>(bencode["a"]["b"]));
    REQUIRE(NRef<String>(bencode["a"]["b"][0]).value() == "x");
    REQUIRE(NRef<String>(bencode["a"]["b"][1]).value() == "y");
    REQUIRE(NRef<String>(bencode["a"]["b"][2]).value() == "z");
    BufferDestination destination;
    REQUIRE_NOTHROW(bencode.stringify(destination));
    REQUIRE(destination.toString() == R"(d1:ad1:bl1:x1:y1:zeee)");
  }
}