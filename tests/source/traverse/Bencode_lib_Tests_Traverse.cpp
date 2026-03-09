#include "Bencode_Lib_Tests.hpp"

class Bencode_Analyzer final : public IAction {
public:
  Bencode_Analyzer() = default;
  ~Bencode_Analyzer() override = default;
  void onNode([[maybe_unused]] const Node &bNode) override { totalNodes++; }
  void onString([[maybe_unused]] const Node &bNode) override { totalStrings++; }
  void onInteger([[maybe_unused]] const Node &bNode) override {
    totalIntegers++;
  }
  void onList([[maybe_unused]] const Node &bNode) override { totalLists++; }
  void onDictionary([[maybe_unused]] const Node &bNode) override {
    totalDictionarys++;
  }

  Bencode::IntegerType totalNodes{};
  Bencode::IntegerType totalDictionarys{};
  Bencode::IntegerType totalLists{};
  Bencode::IntegerType totalStrings{};
  Bencode::IntegerType totalIntegers{};
};
TEST_CASE("Bencode Node tree traverse tests ", "[Bencode][Traverse]") {
  const Bencode bStringify;
  SECTION("Parse an Integer and traverse", "[Bencode][Traverse][Integer]") {
    BufferSource source{"i266e"};
    bStringify.parse(source);
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalIntegers == 1);
    REQUIRE(analyzer.totalDictionarys == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalLists == 0);
  }
  SECTION("Parse an String and traverse", "[Bencode][Traverse][String]") {
    BufferSource source{"11:Test String"};
    bStringify.parse(source);
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalIntegers == 0);
    REQUIRE(analyzer.totalDictionarys == 0);
    REQUIRE(analyzer.totalStrings == 1);
    REQUIRE(analyzer.totalLists == 0);
  }
  SECTION("Parse an List and traverse", "[Bencode][Traverse][List]") {
    BufferSource source{"li1ei2ei3ei4ee"};
    bStringify.parse(source);
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 5);
    REQUIRE(analyzer.totalIntegers == 4);
    REQUIRE(analyzer.totalDictionarys == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalLists == 1);
  }
  SECTION("Parse an Dictionary and traverse",
          "[Bencode][Traverse][Dictionary]") {
    BufferSource source{"d3:onei1e5:threei3e3:twoi2ee"};
    bStringify.parse(source);
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 4);
    REQUIRE(analyzer.totalIntegers == 3);
    REQUIRE(analyzer.totalDictionarys == 1);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalLists == 0);
  }
  SECTION("Parse a complex tree and traverse",
          "[Bencode][Traverse][Dictionary]") {
    BufferSource source{
        R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)"};
    bStringify.parse(source);
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 14);
    REQUIRE(analyzer.totalIntegers == 8);
    REQUIRE(analyzer.totalDictionarys == 3);
    REQUIRE(analyzer.totalStrings == 2);
    REQUIRE(analyzer.totalLists == 1);
  }
  SECTION("Parse an empty list and traverse.", "[Bencode][Traverse][List]") {
    bStringify.parse(BufferSource{"le"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalLists == 1);
    REQUIRE(analyzer.totalIntegers == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalDictionarys == 0);
  }
  SECTION("Parse an empty dictionary and traverse.",
          "[Bencode][Traverse][Dictionary]") {
    bStringify.parse(BufferSource{"de"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalDictionarys == 1);
    REQUIRE(analyzer.totalIntegers == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalLists == 0);
  }
  SECTION("Parse a list of strings and traverse.",
          "[Bencode][Traverse][List]") {
    bStringify.parse(BufferSource{"l3:foo3:bar3:baze"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 4);
    REQUIRE(analyzer.totalLists == 1);
    REQUIRE(analyzer.totalStrings == 3);
    REQUIRE(analyzer.totalIntegers == 0);
    REQUIRE(analyzer.totalDictionarys == 0);
  }
  SECTION("Parse a dictionary of strings and traverse.",
          "[Bencode][Traverse][Dictionary]") {
    bStringify.parse(BufferSource{"d1:a3:foo1:b3:bare"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 3);
    REQUIRE(analyzer.totalDictionarys == 1);
    REQUIRE(analyzer.totalStrings == 2);
    REQUIRE(analyzer.totalIntegers == 0);
    REQUIRE(analyzer.totalLists == 0);
  }
  SECTION("Parse a nested list and traverse.", "[Bencode][Traverse][List]") {
    // outer list + inner list + 2 integers = 4 nodes
    bStringify.parse(BufferSource{"lli1ei2eee"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 4);
    REQUIRE(analyzer.totalLists == 2);
    REQUIRE(analyzer.totalIntegers == 2);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalDictionarys == 0);
  }
  SECTION("Parse a dictionary containing a list and traverse.",
          "[Bencode][Traverse]") {
    // dict + list + 3 integers = 5 nodes
    bStringify.parse(BufferSource{"d4:numsli1ei2ei3eee"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 5);
    REQUIRE(analyzer.totalDictionarys == 1);
    REQUIRE(analyzer.totalLists == 1);
    REQUIRE(analyzer.totalIntegers == 3);
    REQUIRE(analyzer.totalStrings == 0);
  }
  SECTION("Parse a list containing a dictionary and traverse.",
          "[Bencode][Traverse]") {
    // list + dict + 1 integer = 3 nodes
    bStringify.parse(BufferSource{"ld3:agei30eee"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 3);
    REQUIRE(analyzer.totalLists == 1);
    REQUIRE(analyzer.totalDictionarys == 1);
    REQUIRE(analyzer.totalIntegers == 1);
    REQUIRE(analyzer.totalStrings == 0);
  }
  SECTION("Parse a mixed list and traverse.", "[Bencode][Traverse][List]") {
    // list + 2 integers + 2 strings = 5 nodes
    bStringify.parse(BufferSource{"li1e3:twoi3e4:foure"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 5);
    REQUIRE(analyzer.totalLists == 1);
    REQUIRE(analyzer.totalIntegers == 2);
    REQUIRE(analyzer.totalStrings == 2);
    REQUIRE(analyzer.totalDictionarys == 0);
  }
  SECTION("Parse single integer and check node count is one.",
          "[Bencode][Traverse][Integer]") {
    bStringify.parse(BufferSource{"i-99e"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalIntegers == 1);
  }
  SECTION("Parse single string and check node count is one.",
          "[Bencode][Traverse][String]") {
    bStringify.parse(BufferSource{"0:"});
    Bencode_Analyzer analyzer;
    bStringify.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalStrings == 1);
  }
}