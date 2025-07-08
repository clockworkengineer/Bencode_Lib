#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Check R-Value reference stringify/parse.",
          "[Bencode][Node][Reference]") {
  const Bencode bStringify;
  SECTION("Stringify/Parse with R-Value reference (Buffer).",
          "[Bencode][Node][R-Value Reference]") {
    bStringify.parse(BufferSource{"i45500e"});
    bStringify.stringify(
        BufferDestination{}); // Does nothing as sink (for completeness)
    REQUIRE(NRef<Integer>((bStringify.root())).value() == 45500);
  }
  SECTION("Stringify/Parse both with R-Value reference (File).",
          "[Bencode][Node][R-alue Reference]") {
    bStringify.parse(FileSource{prefixTestDataPath(kMultiFileTorrent)});
    FileDestination destination{generateRandomFileName()};
    bStringify.stringify(destination);
    destination.close();
    REQUIRE_FALSE(!compareFiles(prefixTestDataPath(kMultiFileTorrent),
                                destination.getFileName()));
    std::filesystem::remove(destination.getFileName());
  }
}