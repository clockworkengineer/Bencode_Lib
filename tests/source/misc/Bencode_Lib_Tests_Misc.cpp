#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check R-Value reference stringify/parse.",
          "[Bencode][BNode][Reference]") {
  const Bencode bStringify;
  SECTION("Stringify/Parse with R-Value reference (Buffer).",
          "[Bencode][BNode][R-Value Reference]") {
    bStringify.parse(BufferSource{"i45500e"});
    bStringify.stringify(
        BufferDestination{}); // Does nothing as sink (for completeness)
    REQUIRE(BRef<Integer>((bStringify.root())).value() == 45500);
  }
  SECTION("Stringify/Parse both with R-Value reference (File).",
          "[Bencode][BNode][R-alue Reference]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    bStringify.parse(FileSource{prefixTestDataPath(kMultiFileTorrent)});
    bStringify.stringify(FileDestination{prefixTestDataPath(kGeneratedTorrentFile)});
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}