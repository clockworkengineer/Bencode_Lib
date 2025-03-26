#include "Bencode_Lib_Tests.hpp"

class Bencode_Analyzer final : public IAction {
public:
  Bencode_Analyzer() = default;
  ~Bencode_Analyzer() override = default;
  void onBNode([[maybe_unused]] const BNode &bNode) override {
    totalNodes++;
  }
  void onString([[maybe_unused]] const BNode &bNode) override {
    totalStrings++;
  }
  void onInteger([[maybe_unused]] const BNode &bNode) override {
    totalIntegers++;
  }
  void onList([[maybe_unused]] const BNode &bNode) override {
    totalLists++;
  }
  void onDictionary([[maybe_unused]] const BNode &bNode) override {
    totalDictionarys++;
  }

  int64_t totalNodes{};
  int64_t totalDictionarys{};
  int64_t totalLists{};
  int64_t totalStrings{};
  int64_t totalIntegers{};
};
TEST_CASE("Bencode BNode tree traverse tests ", "[Bencode][Traverse]") {
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
      BufferSource
      source{R"(d6:answerd10:everythingi42ee10:dictionaryd8:currency3:USD5:valuei42ee5:happyi1e4:listli1ei0ei2ee4:name5:Niels7:nothingi0e2:pii3ee)"};
      bStringify.parse(source);
      Bencode_Analyzer analyzer;
      bStringify.traverse(analyzer);
      REQUIRE(analyzer.totalNodes == 14);
      REQUIRE(analyzer.totalIntegers == 8);
      REQUIRE(analyzer.totalDictionarys == 3);
      REQUIRE(analyzer.totalStrings == 2);
      REQUIRE(analyzer.totalLists == 1);
    }
}