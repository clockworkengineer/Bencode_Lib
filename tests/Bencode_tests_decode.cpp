//
// Unit Tests: Bencode
//
// Description: Decode unit tests for Bencode class using the Catch2 test framework.
//
// ================
// Test definitions
// ================
#include "Bencode_tests.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ==========
// Test cases
// ==========
TEST_CASE("Bencode for decode of simple types (number, string) ", "[Bencode][Decode]")
{
  Bencode bEncode;
  SECTION("Decode an integer", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "i266e" };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::integer);
  }
  SECTION("Decode an string", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "12:qwertyuiopas" };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::string);
  }
  SECTION("Decode an integer (266) and check value", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "i266e" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeInteger>(*bEncode.getRoot()).getInteger() == 266);
  }
  SECTION("Decode an integer (1000) and check value", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "i1000e" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeInteger>(*bEncode.getRoot()).getInteger() == 1000);
  }
  SECTION("Decode an negative integer (-666) and check value", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "i-666e" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeInteger>(*bEncode.getRoot()).getInteger() == -666);
  }
  SECTION("Decode an string ('qwertyuiopas') and check value", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "12:qwertyuiopas" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>(*bEncode.getRoot()).getString() == "qwertyuiopas");
  }
  SECTION("Decode an string ('abcdefghijklmnopqrstuvwxyz') and check value", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "26:abcdefghijklmnopqrstuvwxyz" };
    bEncode.decode(bEncodeSource);
    REQUIRE(BNodeRef<BNodeString>(*bEncode.getRoot()).getString() == "abcdefghijklmnopqrstuvwxyz");
  }
}
TEST_CASE("Bencode for decode of a table of integer test data", "[Bencode][Decode]")
{
  Bencode bEncode;
  auto [testInput, expected] = GENERATE(table<std::string, long>({{"i277e", 277},
                                                                  {"i32767e", 32767}}));
  BufferSource bEncodeSource { testInput };
  bEncode.decode(bEncodeSource);
  REQUIRE(BNodeRef<BNodeInteger>(*bEncode.getRoot()).getInteger() == expected);
}
TEST_CASE("Bencode for decode of a table of string test data", "[Bencode][Decode]")
{
  Bencode bEncode;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>({{"13:qwertyuiopasd", "qwertyuiopasd"},
                                                                         {"6:mnbvcx", "mnbvcx"}}));
  BufferSource bEncodeSource { testInput };
  bEncode.decode(bEncodeSource);
  REQUIRE(BNodeRef<BNodeString>(*bEncode.getRoot()).getString() == expected);
}
TEST_CASE("Bencode for decode of collection types (list, dictionary) ", "[Bencode][Decode]")
{
  Bencode bEncode;
  SECTION("Decode an List", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "li266ei6780ei88e5:threee" };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::list);
    REQUIRE((*bEncode.getRoot())[0].nodeType == BNodeType::integer);
    REQUIRE((*bEncode.getRoot())[1].nodeType == BNodeType::integer);
    REQUIRE((*bEncode.getRoot())[2].nodeType == BNodeType::integer);
    REQUIRE((*bEncode.getRoot())[3].nodeType == BNodeType::string);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())[0]).getInteger() == 266);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())[1]).getInteger() == 6780);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())[2]).getInteger() == 88);
    REQUIRE(BNodeRef<BNodeString>((*bEncode.getRoot())[3]).getString() == "three");
  }
  SECTION("Decode an Dictionary", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "d3:onei1e5:threei3e3:twoi2ee" };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::dictionary);
    REQUIRE((*bEncode.getRoot())["one"].nodeType == BNodeType::integer);
    REQUIRE((*bEncode.getRoot())["two"].nodeType == BNodeType::integer);
    REQUIRE((*bEncode.getRoot())["three"].nodeType == BNodeType::integer);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())["one"]).getInteger() == 1);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())["two"]).getInteger() == 2);
    REQUIRE(BNodeRef<BNodeInteger>((*bEncode.getRoot())["three"]).getInteger() == 3);
  }
  SECTION("Decode an list of integers and check values", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "li266ei6780ei88ee" };
    bEncode.decode(bEncodeSource);
    std::vector<long> numbers;
    for (const auto &bNode : BNodeRef<BNodeList>(*bEncode.getRoot()).getArray())
    {
      numbers.push_back(BNodeRef<BNodeInteger>(*bNode).getInteger());
    }
    REQUIRE(numbers == std::vector<long>{266, 6780, 88});
  }
  SECTION("Decode an list of strings and check values", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze" };
    bEncode.decode(bEncodeSource);
    std::vector<std::string> strings;
    for (const auto &bNode : BNodeRef<BNodeList>(*bEncode.getRoot()).getArray())
    {
      strings.push_back(BNodeRef<BNodeString>(*bNode).getString());
    }
    REQUIRE(strings == std::vector<std::string>{"sillyy", "poiuytrewqas", "abcdefghijklmnopqrstuvwxyz"});
  }
  SECTION("Decode an Dictionary of ints and check values", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "d3:onei1e5:threei3e3:twoi2ee" };
    bEncode.decode(bEncodeSource);
    std::map<std::string, long> entries;
    for (const auto &bNode : BNodeRef<BNodeDict>(*bEncode.getRoot()).getDict())
    {
      entries[bNode.first] = BNodeRef<BNodeInteger>(*bNode.second).getInteger();
    }
    REQUIRE(entries == std::map<std::string, long>{{"one", 1}, {"two", 2}, {"three", 3}});
  }
  SECTION("Decode a Dictionary of strings and check values", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle" };
    bEncode.decode(bEncodeSource);
    std::map<std::string, std::string> entries;
    for (const auto &bNode : BNodeRef<BNodeDict>(*bEncode.getRoot()).getDict())
    {
      entries[bNode.first] = BNodeRef<BNodeString>(*bNode.second).getString();
    }
    REQUIRE(entries == std::map<std::string, std::string>{{"one", "0123456789"}, {"two", "asdfghjkl"}, {"three", "qwerty"}});
  }
}
TEST_CASE("Decode generated exceptions", "[Bencode][Decode][Exceptions]")
{
  Bencode bEncode;
  SECTION("Decode an string without terminating ':' on its length", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "26abcdefghijklmnopqrstuvwxyz" };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
  SECTION("Decode an integer without a terminating end", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "i266" };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
  SECTION("Decode an list without a terminating end", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "li266ei6780ei88e" };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
  SECTION("Decode an diictionary without a terminating end", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "d3:one10:01234567895:three6:qwerty3:two9:asdfghjkl" };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
  SECTION("Decode an string that terminates prematurely", "[Bencode][Decode]")
  {
    BufferSource bEncodeSource { "26:abcdefghijklmno" };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Decode buffer empty before decode complete.");
  }
}
TEST_CASE("Decode torrent files using decode", "[Bencode][Decode][Torrents]")
{
  Bencode bEncode;
  SECTION("Decode singlefile.torrent", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kSingleFileTorrent) };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::dictionary);
  }
  SECTION("Decode singlefile.torrent and check value ", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kSingleFileTorrent) };
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bEncodeDestination.getBuffer() == Bencoding(readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent))));
  }
  SECTION("Decode multifile.torrent", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kMultiFileTorrent) };
    bEncode.decode(bEncodeSource);
    REQUIRE(bEncode.getRoot()->nodeType == BNodeType::dictionary);
  }
  SECTION("Decode multifile.torrent and check value ", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kMultiFileTorrent) };
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bEncodeDestination.getBuffer() == Bencoding(readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent))));
  }
}
TEST_CASE("Decode erronous torrent files using decode", "[Bencode][Decode][Torrents]")
{
  Bencode bEncode;
  SECTION("Decode singlefileerror.torrent", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kSingleFileWithErrorTorrent) };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
  SECTION("Decode multifileerror.torrent", "[Bencode][Decode][Torrents]")
  {
    FileSource bEncodeSource { prefixTestDataPath(kMultiFileWithErrorTorrent) };
    REQUIRE_THROWS_WITH(bEncode.decode(bEncodeSource), "Bencoding syntax error detected.");
  }
}