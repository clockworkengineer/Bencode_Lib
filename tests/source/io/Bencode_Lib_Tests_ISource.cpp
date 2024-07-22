//
// Unit Tests: Bencode_Lib_Tests_ISource
//
// Description: ISource unit tests for Bencode class
// using the Catch2 test framework.
//

#include "Bencode_Lib_Tests.hpp"

TEST_CASE("ISource (Buffer interface). Contains file singlefile.torrent.",
          "[Bencode][ISource]") {
  std::string bencodedBuffer{
      readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent))};
  SECTION("Create BufferSource.", "[Bencode][ISource]") {
    REQUIRE_NOTHROW(BufferSource(bencodedBuffer));
  }
  SECTION("Create BufferSource with empty buffer.", "[Bencode][ISource]") {
    REQUIRE_THROWS_AS(BufferSource(""), ISource::Error);
    REQUIRE_THROWS_WITH(
        BufferSource(""),
        "ISource Error: Empty source buffer passed to be decoded.");
  }
  SECTION("Create BufferSource with singlefile.torrent and that it is "
          "positioned on the correct first character",
          "[Bencode][ISource]") {
    BufferSource source{bencodedBuffer};
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("Create BufferSource with singlefile.torrent and then check "
          "moveToNextByte positions to correct next character",
          "[Bencode][ISource]") {
    BufferSource source{bencodedBuffer};
    source.next();
    REQUIRE_FALSE(!source.more());
    REQUIRE((char)source.current() == '8');
  }
  SECTION("Create BufferSource with singlefile.torrent move past last "
          "character, check it and the bytes moved.",
          "[Bencode][ISource]") {
    BufferSource source{bencodedBuffer};
    int64_t length = 0;
    while (source.more()) {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                              // eof
    REQUIRE(source.current() == static_cast<char>(255)); // eof
  }
}
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
    int64_t length = 0;
    while (source.more()) {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                               // eof
    REQUIRE(source.current() == static_cast<char>(0xff)); // eof
  }
}
