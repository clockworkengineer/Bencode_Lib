#include "Bencode_Lib_Tests.hpp"

TEST_CASE("ISource (File interface).", "[Bencode][ISource]") {
  SECTION("Create FileSource with singlefile.torrent.", "[Bencode][ISource]") {
    REQUIRE_NOTHROW(FileSource(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Create FileSource with non existant file.", "[Bencode][ISource]") {
    REQUIRE_THROWS_AS(FileSource(prefixTestDataPath(kNonExistantTorrent)),
                      ISource::Error);
    REQUIRE_THROWS_WITH(FileSource(prefixTestDataPath(kNonExistantTorrent)),
                        "ISource Error: Bencode file input stream failed to "
                        "open or does not exist.");
  }
  SECTION("Create FileSource with singlefile.torrent. and positioned on the "
          "correct first character",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create FileSource with singlefile.torrent and then check "
          "moveToNextByte positions to correct next character",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create FileSource with singlefile.torrent move past last character, "
          "check it and the bytes moved.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    Bencode::IntegerType length = 0;
    while (source.more()) {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                               // eof
    REQUIRE(source.current() == static_cast<char>(0xff)); // eof
  }
  SECTION("reset() repositions FileSource back to the first character.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    source.next();
    source.next();
    source.reset();
    REQUIRE(source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("more() returns false after exhausting the file.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    while (source.more()) {
      source.next();
    }
    REQUIRE_FALSE(source.more());
  }
  SECTION("reset() after full traversal allows re-reading from start.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    while (source.more()) {
      source.next();
    }
    source.reset();
    REQUIRE(source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Traversing singlefile.torrent collects correct total byte count.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    Bencode::IntegerType count = 0;
    while (source.more()) {
      source.current();
      source.next();
      count++;
    }
    REQUIRE(count == 764);
  }
  SECTION("getFileName() returns the path used to open the source.",
          "[Bencode][ISource]") {
    const std::string path{prefixTestDataPath(kSingleFileTorrent)};
    FileSource source{path};
    REQUIRE(source.getFileName() == path);
  }
  SECTION("Parse singlefile.torrent via FileSource and verify root is a "
          "dictionary.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(
        bencode.parse(FileSource{prefixTestDataPath(kSingleFileTorrent)}));
    REQUIRE(isA<Dictionary>(bencode.root()));
  }
  SECTION(
      "Parse multifile.torrent via FileSource and verify root is a dictionary.",
      "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(
        bencode.parse(FileSource{prefixTestDataPath(kMultiFileTorrent)}));
    REQUIRE(isA<Dictionary>(bencode.root()));
  }
  SECTION("Parse singlefile.torrent via FileSource then stringify into buffer "
          "and re-parse — roots match in type.",
          "[Bencode][ISource]") {
    Bencode bencode;
    bencode.parse(FileSource{prefixTestDataPath(kSingleFileTorrent)});
    BufferDestination dst;
    bencode.stringify(dst);
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(BufferSource{dst.toString()}));
    REQUIRE(isA<Dictionary>(bencode2.root()));
  }
  SECTION("Parse singlefile.torrent twice using reset() yields identical "
          "stringify output.",
          "[Bencode][ISource]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    Bencode bencode;
    bencode.parse(source);
    BufferDestination dst1;
    bencode.stringify(dst1);
    source.reset();
    bencode.parse(source);
    BufferDestination dst2;
    bencode.stringify(dst2);
    REQUIRE(dst1.toString() == dst2.toString());
  }
}
