//
// Unit Tests: Bencode
//
// Description: Miscellaneous unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

using namespace Bencode_Lib;

TEST_CASE("Check R-Value reference encode/decode.",
          "[Bencode][BNode][Reference]") {
  const Bencode bEncode;
  SECTION("Encode/Decode with R-Value reference (Buffer).",
          "[Bencode][BNode][R-Value Reference]") {
    bEncode.decode(BufferSource{"i45500e"});
    bEncode.encode(
        BufferDestination{}); // Does nothing as sink (for completeness)
    REQUIRE(BRef<Integer>((bEncode.root())).value() == 45500);
  }
  SECTION("Encode/Decode both with R-Value reference (File).",
          "[Bencode][BNode][R-alue Reference]") {
    std::filesystem::remove(prefixTestDataPath(kGeneratedTorrentFile));
    bEncode.decode(FileSource{prefixTestDataPath(kMultiFileTorrent)});
    bEncode.encode(FileDestination{prefixTestDataPath(kGeneratedTorrentFile)});
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                prefixTestDataPath(kGeneratedTorrentFile)));
  }
}