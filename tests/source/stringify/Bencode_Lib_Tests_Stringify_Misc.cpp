#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Stringify generated exceptions",
          "[Bencode][Stringify][Exceptions]") {
  SECTION("Stringify called with no Node tree to stringify",
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
TEST_CASE("Stringify to buffer round-trips correctly",
          "[Bencode][Stringify][Buffer]") {
  const Bencode bStringify;
  SECTION("Integer round-trips to buffer.", "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i12345e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i12345e");
  }
  SECTION("Negative integer round-trips to buffer.",
          "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"i-99e"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "i-99e");
  }
  SECTION("String round-trips to buffer.", "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"5:hello"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "5:hello");
  }
  SECTION("Empty string round-trips to buffer.",
          "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"0:"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "0:");
  }
  SECTION("List round-trips to buffer.", "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"li1ei2ei3ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "li1ei2ei3ee");
  }
  SECTION("Dictionary round-trips to buffer.", "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"d1:ai1e1:bi2ee"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "d1:ai1e1:bi2ee");
  }
  SECTION("Empty list round-trips to buffer.", "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"le"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "le");
  }
  SECTION("Empty dictionary round-trips to buffer.",
          "[Bencode][Stringify][Buffer]") {
    BufferDestination destination;
    bStringify.parse(BufferSource{"de"});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == "de");
  }
}
TEST_CASE("Stringify to file round-trips correctly",
          "[Bencode][Stringify][File]") {
  const Bencode bStringify;
  SECTION("Integer round-trips via file.", "[Bencode][Stringify][File]") {
    const auto fname = generateRandomFileName();
    bStringify.parse(BufferSource{"i42e"});
    bStringify.stringify(FileDestination{fname});
    Bencode bRead;
    BufferDestination dest;
    bRead.parse(FileSource{fname});
    bRead.stringify(dest);
    std::filesystem::remove(fname);
    REQUIRE(dest.toString() == "i42e");
  }
  SECTION("Dictionary round-trips via file.", "[Bencode][Stringify][File]") {
    const auto fname = generateRandomFileName();
    bStringify.parse(BufferSource{"d3:agei25e4:name5:Alicee"});
    bStringify.stringify(FileDestination{fname});
    Bencode bRead;
    BufferDestination dest;
    bRead.parse(FileSource{fname});
    bRead.stringify(dest);
    std::filesystem::remove(fname);
    REQUIRE(dest.toString() == "d3:agei25e4:name5:Alicee");
  }
}