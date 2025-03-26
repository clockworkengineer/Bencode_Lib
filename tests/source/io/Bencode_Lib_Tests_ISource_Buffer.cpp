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
        "ISource Error: Empty source buffer passed to be parsed.");
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
