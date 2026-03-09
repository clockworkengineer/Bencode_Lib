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
    Bencode::IntegerType length = 0;
    while (source.more()) {
      source.next();
      length++;
    }
    REQUIRE(length == 764);                              // eof
    REQUIRE(source.current() == static_cast<char>(255)); // eof
  }
  SECTION("reset() repositions source back to the first character.",
          "[Bencode][ISource]") {
    BufferSource source{bencodedBuffer};
    source.next();
    source.next();
    source.reset();
    REQUIRE(source.more());
    REQUIRE((char)source.current() == 'd');
  }
  SECTION("more() returns false after exhausting the buffer.",
          "[Bencode][ISource]") {
    BufferSource source{"i1e"};
    while (source.more()) {
      source.next();
    }
    REQUIRE_FALSE(source.more());
  }
  SECTION("next() throws when called on an exhausted buffer.",
          "[Bencode][ISource]") {
    BufferSource source{"i1e"};
    while (source.more()) {
      source.next();
    }
    REQUIRE_THROWS_AS(source.next(), ISource::Error);
  }
  SECTION("Traverse every character of a small known buffer and collect them.",
          "[Bencode][ISource]") {
    BufferSource source{"li1ei2ee"};
    std::string collected;
    while (source.more()) {
      collected += source.current();
      source.next();
    }
    REQUIRE(collected == "li1ei2ee");
  }
  SECTION("reset() after full traversal allows re-reading from start.",
          "[Bencode][ISource]") {
    BufferSource source{"i42e"};
    while (source.more()) {
      source.next();
    }
    source.reset();
    REQUIRE(source.more());
    REQUIRE((char)source.current() == 'i');
  }
  SECTION("Parse a simple integer from BufferSource via Bencode.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"i99e"}));
    REQUIRE(NRef<Integer>(bencode.root()).value() == 99);
  }
  SECTION("Parse a simple string from BufferSource via Bencode.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"5:hello"}));
    REQUIRE(NRef<String>(bencode.root()).value() == "hello");
  }
  SECTION("Parse a list from BufferSource via Bencode.", "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"li1ei2ei3ee"}));
    REQUIRE(isA<List>(bencode.root()));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 1);
    REQUIRE(NRef<Integer>(bencode[1]).value() == 2);
    REQUIRE(NRef<Integer>(bencode[2]).value() == 3);
  }
  SECTION("Parse a dictionary from BufferSource via Bencode.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"d3:key5:valuee"}));
    REQUIRE(isA<Dictionary>(bencode.root()));
    REQUIRE(NRef<String>(bencode["key"]).value() == "value");
  }
  SECTION("Parse a negative integer from BufferSource via Bencode.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"i-100e"}));
    REQUIRE(NRef<Integer>(bencode.root()).value() == -100);
  }
  SECTION("Parse an empty string from BufferSource via Bencode.",
          "[Bencode][ISource]") {
    Bencode bencode;
    REQUIRE_NOTHROW(bencode.parse(BufferSource{"0:"}));
    REQUIRE(NRef<String>(bencode.root()).value().empty());
  }
}
