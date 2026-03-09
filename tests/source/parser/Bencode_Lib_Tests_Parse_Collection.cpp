#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of collection types (list, dictionary) ",
          "[Bencode][Parse][List]") {
  const Bencode bStringify;
  SECTION("Parse an List", "[Bencode][Parse]") {
    BufferSource source{"li266ei6780ei88e5:threee"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<List>(bStringify.root()));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root()[1]));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root()[2]));
    REQUIRE_FALSE(!isA<String>(bStringify.root()[3]));
    REQUIRE(NRef<Integer>((bStringify.root())[0]).value() == 266);
    REQUIRE(NRef<Integer>((bStringify.root())[1]).value() == 6780);
    REQUIRE(NRef<Integer>((bStringify.root())[2]).value() == 88);
    REQUIRE(NRef<String>((bStringify.root())[3]).value() == "three");
  }
  SECTION("Parse an list of integers.s", "[Bencode][Parse][List]") {
    BufferSource source{"li266ei6780ei88ee"};
    bStringify.parse(source);
    std::vector<Bencode::IntegerType> numbers;
    for (const auto &bNode : NRef<List>(bStringify.root()).value()) {
      numbers.push_back(NRef<Integer>(bNode).value());
    }
    REQUIRE(numbers == std::vector<Bencode::IntegerType>{266, 6780, 88});
  }
  SECTION("Parse an list of strings.s", "[Bencode][Parse][List]") {
    BufferSource source{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    bStringify.parse(source);
    std::vector<std::string> strings;
    for (const auto &bNode : NRef<List>(bStringify.root()).value()) {
      strings.push_back(std::string(NRef<String>(bNode).value()));
    }
    REQUIRE(strings == std::vector<std::string>{"sillyy", "poiuytrewqas",
                                                "abcdefghijklmnopqrstuvwxyz"});
  }
  SECTION("Parse an Dictionary", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bStringify.parse(source);
    REQUIRE_FALSE(!isA<Dictionary>(bStringify.root()));
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["one"]);
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["two"]);
    REQUIRE_FALSE(!isA<Integer>(bStringify.root())["three"]);
    REQUIRE(NRef<Integer>((bStringify.root())["one"]).value() == 1);
    REQUIRE(NRef<Integer>((bStringify.root())["two"]).value() == 2);
    REQUIRE(NRef<Integer>((bStringify.root())["three"]).value() == 3);
  }
  SECTION("Parse an Dictionary of ints.s", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bStringify.parse(source);
    std::map<std::string_view, Bencode::IntegerType> entries;
    for (const auto &bNode : NRef<Dictionary>(bStringify.root()).value()) {
      entries[bNode.getKey()] = NRef<Integer>(bNode.getNode()).value();
    }
    REQUIRE(entries == std::map<std::string_view, Bencode::IntegerType>{
                           {"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Parse a Dictionary of strings.s", "[Bencode][Parse][Dictionary]") {
    BufferSource source{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    bStringify.parse(source);
    std::map<std::string_view, std::string_view> entries;
    for (const auto &bNode : NRef<Dictionary>(bStringify.root()).value()) {
      entries[bNode.getKey()] = NRef<String>(bNode.getNode()).value();
    }
    REQUIRE(entries ==
            std::map<std::string_view, std::string_view>{{"one", "0123456789"},
                                                         {"two", "asdfghjkl"},
                                                         {"three", "qwerty"}});
  }
  SECTION("Parse empty list.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"le"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(NRef<List>(bStringify.root()).size() == 0);
  }
  SECTION("Parse empty dictionary.", "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"de"});
    REQUIRE(isA<Dictionary>(bStringify.root()));
    REQUIRE(NRef<Dictionary>(bStringify.root()).size() == 0);
  }
  SECTION("Parse list containing a single string.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"l5:helloe"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(NRef<List>(bStringify.root()).size() == 1);
    REQUIRE(NRef<String>(bStringify.root()[0]).value() == "hello");
  }
  SECTION("Parse list containing a single integer.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"li-7ee"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(NRef<List>(bStringify.root()).size() == 1);
    REQUIRE(NRef<Integer>(bStringify.root()[0]).value() == -7);
  }
  SECTION("Parse list containing a nested list.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"lli1ei2eee"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(isA<List>(bStringify.root()[0]));
    REQUIRE(NRef<Integer>(bStringify.root()[0][0]).value() == 1);
    REQUIRE(NRef<Integer>(bStringify.root()[0][1]).value() == 2);
  }
  SECTION("Parse dictionary with a single integer value.",
          "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"d3:agei30ee"});
    REQUIRE(isA<Dictionary>(bStringify.root()));
    REQUIRE(NRef<Integer>(bStringify.root()["age"]).value() == 30);
  }
  SECTION("Parse dictionary with a single string value.",
          "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"d4:name5:Alicee"});
    REQUIRE(isA<Dictionary>(bStringify.root()));
    REQUIRE(NRef<String>(bStringify.root()["name"]).value() == "Alice");
  }
  SECTION("Parse dictionary containing a list value.",
          "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"d5:itemsli1ei2ei3eee"});
    REQUIRE(isA<Dictionary>(bStringify.root()));
    REQUIRE(isA<List>(bStringify.root()["items"]));
    REQUIRE(NRef<List>(bStringify.root()["items"]).size() == 3);
    REQUIRE(NRef<Integer>(bStringify.root()["items"][0]).value() == 1);
    REQUIRE(NRef<Integer>(bStringify.root()["items"][2]).value() == 3);
  }
  SECTION("Parse list containing a dictionary.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"ld3:keyi99eee"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(isA<Dictionary>(bStringify.root()[0]));
    REQUIRE(NRef<Integer>(bStringify.root()[0]["key"]).value() == 99);
  }
  SECTION("Parse dictionary keys are in sorted order.",
          "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"d1:ai1e1:bi2e1:ci3ee"});
    const auto &entries = NRef<Dictionary>(bStringify.root()).value();
    REQUIRE(entries[0].getKey() == "a");
    REQUIRE(entries[1].getKey() == "b");
    REQUIRE(entries[2].getKey() == "c");
  }
  SECTION("Parse list with mixed types.", "[Bencode][Parse][List]") {
    bStringify.parse(BufferSource{"li42e5:helloe"});
    REQUIRE(isA<List>(bStringify.root()));
    REQUIRE(NRef<List>(bStringify.root()).size() == 2);
    REQUIRE(NRef<Integer>(bStringify.root()[0]).value() == 42);
    REQUIRE(NRef<String>(bStringify.root()[1]).value() == "hello");
  }
  SECTION("Parse nested dictionary.", "[Bencode][Parse][Dictionary]") {
    bStringify.parse(BufferSource{"d5:outerd5:inner4:deepee"});
    REQUIRE(isA<Dictionary>(bStringify.root()));
    REQUIRE(isA<Dictionary>(bStringify.root()["outer"]));
    REQUIRE(NRef<String>(bStringify.root()["outer"]["inner"]).value() ==
            "deep");
  }
}