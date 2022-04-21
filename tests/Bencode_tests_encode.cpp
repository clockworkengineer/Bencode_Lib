//
// Unit Tests: Bencode
//
// Description: Encode unit tests for Bencode class using the Catch2 test framework.
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
TEST_CASE("Bencode for encode of simple types (integer, string) ", "[Bencode][Encode]")
{
    Bencode bEncode;
    SECTION("Encode an integer (266) and check value", "[Bencode][Encode][Integer]")
    {
        BufferSource bEncodeSource{"i266e"};
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "i266e");
    }
    SECTION("Encode an integer (10000) and check value", "[Bencode][Encode][Integer]")
    {
        BufferSource bEncodeSource{"i10000e"};
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "i10000e");
    }
    SECTION("Encode an string ('qwertyuiopas') and check its value", "[Bencode][Encode][String]")
    {
        BufferSource bEncodeSource{"12:qwertyuiopas"};
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "12:qwertyuiopas");
    }
    SECTION("Encode an string ('abcdefghijklmnopqrstuvwxyz') and check its value", "[Bencode][Encode][String]")
    {
        BufferSource bEncodeSource{"26:abcdefghijklmnopqrstuvwxyz"};
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == "26:abcdefghijklmnopqrstuvwxyz");
    }
}
TEST_CASE("Bencode for encode of a table of integer test data", "[Bencode][Encode][Integer]")
{
    Bencode bEncode;
    auto [expected] = GENERATE(table<std::string>({"i277e",
                                                   "i32767e"}));
    BufferSource bEncodeSource(expected);
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
}
TEST_CASE("Bencode for encode of a table of string test data", "[Bencode][Encode][String]")
{
    Bencode bEncode;
    auto [expected] = GENERATE(table<std::string>({"13:qwertyuiopasd",
                                                   "6:mnbvcx"}));
    BufferSource bEncodeSource(expected);
    BufferDestination bEncodeDestination;
    bEncode.decode(bEncodeSource);
    bEncode.encode(bEncodeDestination);
    REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
}
TEST_CASE("Bencode for encode of collection types (list, dictionary) ", "[Bencode][Encode]")
{
    Bencode bEncode;
    SECTION("Encode an List of integers('li266ei6780ei88ee') and check value", "[Bencode][Encode][List]")
    {
        std::string expected{"li266ei6780ei88ee"};
        BufferSource bEncodeSource(expected);
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
    }
    SECTION("Encode an List of strings ('l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze') and check value", "[Bencode][Encode][List]")
    {
        std::string expected{"l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
        BufferSource bEncodeSource(expected);
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
    }
    SECTION("Encode an Dictionary of integers and check balue", "[Bencode][Encode][Dictionary]")
    {
        std::string expected{"d3:onei1e5:threei3e3:twoi2ee"};
        BufferSource bEncodeSource(expected);
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
    }
    SECTION("Encode an Dictionary of strings and check balue", "[Bencode][Encode][Dictionary]")
    {
        std::string expected{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
        BufferSource bEncodeSource(expected);
        BufferDestination bEncodeDestination;
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE(bufferToString(bEncodeDestination.getBuffer()) == expected);
    }
}
TEST_CASE("Encode generated exceptions", "[Bencode][Encode][Exceptions]")
{
    Bencode bEncode;
    SECTION("Encode called with no BNode tree to encode", "[Bencode][Encode][Exceptions]")
    {
        BufferDestination bEncodeDestination;
        REQUIRE_THROWS_AS(bEncode.encode(bEncodeDestination), std::runtime_error);
        REQUIRE_THROWS_WITH(bEncode.encode(bEncodeDestination), "No Bencoded data to encode.");
    }
}
TEST_CASE("Encode torrent files using encodeToFile", "[Bencode][Encode][Torrents]")
{
    Bencode bEncode;
    SECTION("Encode singlefile.torrent and check value", "[Bencode][Encode][Torrents]")
    {
        std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
        FileSource bEncodeSource{prefixTestDataPath(kSingleFileTorrent)};
        FileDestination bEncodeDestination{prefixTestDataPath(kGeneratedTorrentFile)};
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kSingleFileTorrent), prefixTestDataPath(kGeneratedTorrentFile)));
    }
    SECTION("Encode multifile.torrent and check value", "[Bencode][Encode][Torrents]")
    {
        std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
        FileSource bEncodeSource{prefixTestDataPath(kMultiFileTorrent)};
        FileDestination bEncodeDestination{prefixTestDataPath(kGeneratedTorrentFile)};
        bEncode.decode(bEncodeSource);
        bEncode.encode(bEncodeDestination);
        REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent), prefixTestDataPath(kGeneratedTorrentFile)));
    }
}