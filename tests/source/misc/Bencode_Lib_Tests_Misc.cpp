#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check R-Value reference encode/parse.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Encode/Parse with R-Value reference (Buffer).",
          "[Bencode][BNode][R-Value Reference]") {
    bEncode.parse(BufferSource{"i45500e"});
    bEncode.encode(
        BufferDestination{}); // Does nothing as sink (for completeness)
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("Encode/Parse both with R-Value reference (File).",
          "[Bencode][BNode][R-alue Reference]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    bEncode.parse(FileSource{prefixTestDataPath(kMultiFileTorrent)});
    bEncode.encode(FileDestination{prefixTestDataPath(kGeneratedTorrentFile)});
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}