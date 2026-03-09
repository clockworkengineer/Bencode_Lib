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

TEST_CASE("Check Bencode::version().", "[Bencode][Version]") {
  SECTION("version() returns a non-empty string.", "[Bencode][Version]") {
    REQUIRE_FALSE(Bencode::version().empty());
  }
  SECTION("version() string contains a '.'.", "[Bencode][Version]") {
    REQUIRE(Bencode::version().find('.') != std::string::npos);
  }
  SECTION("version() is consistent across calls.", "[Bencode][Version]") {
    REQUIRE(Bencode::version() == Bencode::version());
  }
}

TEST_CASE("Check Bencode error handling.", "[Bencode][Error]") {
  SECTION("Parsing an empty buffer throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS_AS(bencode.parse(BufferSource{"x"}), std::exception);
  }
  SECTION("Parsing malformed bencode throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(bencode.parse(BufferSource{"zzz"}));
  }
  SECTION("Parsing truncated integer throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(bencode.parse(BufferSource{"i42"}));
  }
  SECTION("Parsing truncated string throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(bencode.parse(BufferSource{"5:hi"}));
  }
  SECTION("Parsing truncated list throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(bencode.parse(BufferSource{"li1e"}));
  }
  SECTION("Parsing truncated dictionary throws.", "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(bencode.parse(BufferSource{"d3:key"}));
  }
  SECTION("Parsing a non-existent file via FileSource throws.",
          "[Bencode][Error]") {
    Bencode bencode;
    REQUIRE_THROWS(
        bencode.parse(FileSource{prefixTestDataPath(kNonExistantTorrent)}));
  }
}

TEST_CASE("Check Bencode root() access.", "[Bencode][Root]") {
  SECTION("root() on a parsed integer is an Integer node.", "[Bencode][Root]") {
    Bencode bencode;
    bencode.parse(BufferSource{"i99e"});
    REQUIRE(isA<Integer>(bencode.root()));
  }
  SECTION("root() on a parsed string is a String node.", "[Bencode][Root]") {
    Bencode bencode;
    bencode.parse(BufferSource{"5:hello"});
    REQUIRE(isA<String>(bencode.root()));
  }
  SECTION("root() on a parsed list is a List node.", "[Bencode][Root]") {
    Bencode bencode;
    bencode.parse(BufferSource{"li1ei2ee"});
    REQUIRE(isA<List>(bencode.root()));
  }
  SECTION("root() on a parsed dictionary is a Dictionary node.",
          "[Bencode][Root]") {
    Bencode bencode;
    bencode.parse(BufferSource{"d3:keyi1ee"});
    REQUIRE(isA<Dictionary>(bencode.root()));
  }
  SECTION("const root() returns the same value as non-const root().",
          "[Bencode][Root]") {
    Bencode bencode;
    bencode.parse(BufferSource{"i7e"});
    const Bencode &cbencode = bencode;
    REQUIRE(NRef<Integer>(bencode.root()).value() ==
            NRef<Integer>(cbencode.root()).value());
  }
}

TEST_CASE("Check Bencode parse/stringify round-trips.",
          "[Bencode][RoundTrip]") {
  SECTION("Integer round-trips correctly.", "[Bencode][RoundTrip]") {
    Bencode bencode;
    bencode.parse(BufferSource{"i12345e"});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == "i12345e");
  }
  SECTION("Negative integer round-trips correctly.", "[Bencode][RoundTrip]") {
    Bencode bencode;
    bencode.parse(BufferSource{"i-99e"});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == "i-99e");
  }
  SECTION("Zero integer round-trips correctly.", "[Bencode][RoundTrip]") {
    Bencode bencode;
    bencode.parse(BufferSource{"i0e"});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == "i0e");
  }
  SECTION("String round-trips correctly.", "[Bencode][RoundTrip]") {
    Bencode bencode;
    bencode.parse(BufferSource{"5:hello"});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == "5:hello");
  }
  SECTION("Empty string round-trips correctly.", "[Bencode][RoundTrip]") {
    Bencode bencode;
    bencode.parse(BufferSource{"0:"});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == "0:");
  }
  SECTION("Nested dictionary round-trips correctly.", "[Bencode][RoundTrip]") {
    const std::string encoded{R"(d6:answerd10:everythingi42ee4:name5:Nielse)"};
    Bencode bencode;
    bencode.parse(BufferSource{encoded});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == encoded);
  }
  SECTION("singlefile.torrent round-trips via buffer.",
          "[Bencode][RoundTrip]") {
    const std::string original{
        readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent))};
    Bencode bencode;
    bencode.parse(BufferSource{original});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == original);
  }
  SECTION("multifile.torrent round-trips via buffer.", "[Bencode][RoundTrip]") {
    const std::string original{
        readBencodedBytesFromFile(prefixTestDataPath(kMultiFileTorrent))};
    Bencode bencode;
    bencode.parse(BufferSource{original});
    BufferDestination destination;
    bencode.stringify(destination);
    REQUIRE(destination.toString() == original);
  }
}