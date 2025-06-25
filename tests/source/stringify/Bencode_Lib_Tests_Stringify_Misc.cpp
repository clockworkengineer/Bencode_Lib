#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Stringify generated exceptions",
          "[Bencode][Stringify][Exceptions]") {
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
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    FileDestination destination{generateRandomFileName()};
    bStringify.parse(source);
    bStringify.stringify(destination);
    destination.close();
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kSingleFileTorrent),
                                destination.getFileName()));
    std::filesystem::remove(destination.getFileName());
  }
  SECTION("Stringify multifile.torrent.", "[Bencode][Stringify][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    FileDestination destination{generateRandomFileName()};
    bStringify.parse(source);
    bStringify.stringify(destination);
    destination.close();
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                destination.getFileName()));
    std::filesystem::remove(destination.getFileName());
  }
}