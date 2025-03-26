#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Stringify generated exceptions", "[Bencode][Stringify][Exceptions]") {
  SECTION("Stringify called with no BNode tree to stringify",
          "[Bencode][Stringify][Exceptions]") {
    const Bencode bStringify;
    BufferDestination destination;
    REQUIRE_THROWS_AS(bStringify.stringify(destination), Error);
    REQUIRE_THROWS_WITH(bStringify.stringify(destination),
                        "Bencode Error: No Bencoded data to stringify.");
  }
}
TEST_CASE("Stringify torrent files using encodeToFile",
          "[Bencode][Stringify][Torrents]") {
  const Bencode bStringify;
  SECTION("Stringify singlefile.torrent.", "[Bencode][Stringify][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kSingleFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
  SECTION("Stringify multifile.torrent.", "[Bencode][Stringify][Torrents]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    FileDestination destination{prefixTestDataPath(kGeneratedTorrentFile)};
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}