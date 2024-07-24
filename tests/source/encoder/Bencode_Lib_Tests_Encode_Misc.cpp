//
// Unit Tests: Bencode_Lib_Tests_Encode_Misc
//
// Description: Encode unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Encode generated exceptions", "[Bencode][Encode][Exceptions]") {
  SECTION("Encode called with no BNode tree to encode",
          "[Bencode][Encode][Exceptions]") {
    const Bencode bEncode;
    BufferDestination destination;
    REQUIRE_THROWS_AS(bEncode.encode(destination), Error);
    REQUIRE_THROWS_WITH(bEncode.encode(destination),
                        "Bencode Error: No Bencoded data to encode.");
  }
}
TEST_CASE("Encode torrent files using encodeToFile",
          "[Bencode][Encode][Torrents]") {
  const Bencode bEncode;
  SECTION("Encode singlefile.torrent.", "[Bencode][Encode][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kSingleFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Encode multifile.torrent.", "[Bencode][Encode][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bEncode.decode(source);
    bEncode.encode(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}