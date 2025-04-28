#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for parse of a table of integer test data",
          "[Bencode][Parse][Integer]") {
  const Bencode bEncoder;
  auto [testInput, expected] = GENERATE(
      table<std::string, Bencode::IntegerType>({{"i277e", 277}, {"i32767e", 32767}}));
  BufferSource source{testInput};
  bEncoder.parse(source);
  REQUIRE(BRef<Integer>(bEncoder.root()).value() == expected);
}
TEST_CASE("Bencode for parse of a table of string test data",
          "[Bencode][Parse][String]") {
  const Bencode bEncoder;
  auto [testInput, expected] = GENERATE(table<std::string, std::string>(
      {{"13:qwertyuiopasd", "qwertyuiopasd"}, {"6:mnbvcx", "mnbvcx"}}));
  BufferSource source{testInput};
  bEncoder.parse(source);
  REQUIRE(BRef<String>(bEncoder.root()).value() == expected);
}
TEST_CASE("Parse torrent files", "[Bencode][Parse][Torrents]") {
  const Bencode bEncoder;
  SECTION("Parse singlefile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    bEncoder.parse(source);
    REQUIRE(isA<Dictionary>(bEncoder.root()));
  }
  SECTION("Parse singlefile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kSingleFileTorrent)};
    BufferDestination destination;
    bEncoder.parse(source);
    bEncoder.stringify(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent)));
  }
  SECTION("Parse multifile.torrent", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    bEncoder.parse(source);
    REQUIRE_FALSE(!isA<Dictionary>(bEncoder.root()));
  }
  SECTION("Parse multifile.torrent. ", "[Bencode][Parse][Torrents]") {
    FileSource source{prefixTestDataPath(kMultiFileTorrent)};
    BufferDestination destination;
    bEncoder.parse(source);
    bEncoder.stringify(destination);
    REQUIRE(destination.toString() ==
            readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent)));
  }
}
TEST_CASE("Parse erroneous torrent files",
          "[Bencode][Parse][Torrents][Error]") {
  const Bencode bEncoder;
  SECTION("Parse singlefileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kSingleFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncoder.parse(source), SyntaxError);
  }
  SECTION("Parse multifileerror.torrent",
          "[Bencode][Parse][Torrents][Error]") {
    FileSource source{prefixTestDataPath(kMultiFileWithErrorTorrent)};
    REQUIRE_THROWS_AS(bEncoder.parse(source), SyntaxError);
  }

}
TEST_CASE("Check string overflow handling.", "[Bencode][String][Overflow]") {
  const Bencode bEncoder;
  SECTION("Parse a string with negative length", "[Bencode][Parse][String]") {
    REQUIRE_THROWS_WITH(bEncoder.parse(BufferSource{"-666:"}), "Bencode Syntax Error: Negative string length.");
  }
  SECTION("Parse a string larger than the max allowed length", "[Bencode][Parse][String]") {
    REQUIRE_THROWS_WITH(bEncoder.parse(BufferSource{std::to_string(String::getMaxStringLength()+1)+":"}), "Bencode Syntax Error: String size exceeds maximum allowed size.");
  }
  SECTION("Default max string 16K", "[Bencode][Parse][String]") {
    REQUIRE(String::getMaxStringLength()==16*1024);
  }
  SECTION("Parse a string larger than the max allowed length of 1M", "[Bencode][Parse][String]") {
    String::setMaxStringLength(1024*1024);
    REQUIRE(String::getMaxStringLength()==1024*1024);
    REQUIRE_THROWS_WITH(bEncoder.parse(BufferSource{std::to_string(String::getMaxStringLength()+1)+":"}), "Bencode Syntax Error: String size exceeds maximum allowed size.");
  }

}
TEST_CASE("Check parse depth handling.", "[Bencode][Parse][Depth]") {
  const Bencode bEncoder;
  SECTION("Parse 2 nested list.", "[Bencode][Parse][Depth]") {
    REQUIRE_NOTHROW(bEncoder.parse(BufferSource("llee")));
  }
  SECTION("Parse 11 nested list.", "[Bencode][Parse][Depth]") {
    REQUIRE_THROWS_WITH(bEncoder.parse(BufferSource("lllllllllleeeeeeeeee")),"Bencode Syntax Error: Maximum parser depth exceeded.");
  }
  SECTION("Get default maximum parser depth.", "[Bencode][Parse][Depth]") {
    REQUIRE(Default_Parser::getMaxParserDepth()==10);
  }
  SECTION("Set default maximum parser depth.", "[Bencode][Parse][Depth]") {
    Default_Parser::setMaxParserDepth(20);
    REQUIRE(Default_Parser::getMaxParserDepth()==20);
  }
  SECTION("Set default maximum parser depth and check new value works.", "[Bencode][Parse][Depth]") {
    Default_Parser::setMaxParserDepth(20);
    REQUIRE(Default_Parser::getMaxParserDepth()==20);
    REQUIRE_NOTHROW(bEncoder.parse(BufferSource("llllllllllllllleeeeeeeeeeeeeee")));
  }

}