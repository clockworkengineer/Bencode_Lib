#include "Bencode_Lib_Tests.hpp"
#include <cstddef>
#include <span>

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
  SECTION("Stringify on an empty Bencode object throws.", "[Bencode][Error]") {
    Bencode bencode;
    BufferDestination destination;
    REQUIRE_THROWS_AS(bencode.stringify(destination), std::exception);
  }
  SECTION("Traverse on an empty Bencode object throws.", "[Bencode][Error]") {
    Bencode bencode;
    struct NoOpAction : public IAction {
      void onNode(const Node &) override {}
      void onString(const Node &) override {}
      void onInteger(const Node &) override {}
      void onList(const Node &) override {}
      void onDictionary(const Node &) override {}
    } action;
    REQUIRE_THROWS_AS(bencode.traverse(action), std::exception);
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

TEST_CASE("Check normal build definitions are active.", "[Bencode][Build][Normal]") {
#if defined(BENCODE_ENABLE_EXCEPTIONS) && (BENCODE_ENABLE_EXCEPTIONS == 1)
  SUCCEED("Exception support is enabled in the normal build target");
#else
  FAIL("The normal build target must compile with exceptions enabled");
#endif
#if defined(BENCODE_ENABLE_FILE_IO) && (BENCODE_ENABLE_FILE_IO == 1)
  SUCCEED("File I/O support is enabled in the normal build target");
#else
  FAIL("The normal build target must compile with file I/O enabled");
#endif
}

TEST_CASE("Check Bencode operator[] access.", "[Bencode][Operator]") {
  SECTION("operator[] returns a dictionary value by key.", "[Bencode][Operator][Dictionary]") {
    Bencode bencode;
    bencode.parse(BufferSource{"d3:onei1e3:twoi2ee"});
    REQUIRE(isA<Integer>(bencode["one"]));
    REQUIRE(NRef<Integer>(bencode["one"]).value() == 1);
    REQUIRE(isA<Integer>(bencode["two"]));
    REQUIRE(NRef<Integer>(bencode["two"]).value() == 2);
  }
  SECTION("operator[] returns a list value by index.", "[Bencode][Operator][List]") {
    Bencode bencode;
    bencode.parse(BufferSource{"li1ei2ei3ee"});
    REQUIRE(isA<Integer>(bencode[0]));
    REQUIRE(NRef<Integer>(bencode[0]).value() == 1);
    REQUIRE(NRef<Integer>(bencode[1]).value() == 2);
    REQUIRE(NRef<Integer>(bencode[2]).value() == 3);
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

TEST_CASE("Check Bencode move semantics and convenience APIs.", "[Bencode][API]") {
  SECTION("Move constructor transfers state cleanly.") {
    Bencode original("d3:agei30e4:name4:Johne");
    REQUIRE(original["name"].as_string().value_or("") == "John");

    Bencode moved(std::move(original));
    REQUIRE(moved["name"].as_string().value_or("") == "John");
    REQUIRE(moved["age"].as_int().value_or(0) == 30);
  }

  SECTION("Move assignment transfers state cleanly.") {
    Bencode b1("d3:numi99e5:title4:teste");
    Bencode b2;
    b2 = std::move(b1);
    REQUIRE(b2["title"].as_string().value_or("") == "test");
    REQUIRE(b2["num"].as_int().value_or(0) == 99);
  }

  SECTION("Direct parse(string_view) and stringify() / encode().") {
    Bencode b;
    b.parse("d4:city6:Londone");
    REQUIRE(b["city"].as_string().value_or("") == "London");

    std::string encoded = b.stringify();
    REQUIRE(encoded == "d4:city6:Londone");
    REQUIRE(b.encode() == "d4:city6:Londone");
  }

  SECTION("Operations on moved-from Bencode are safe.") {
    Bencode b1("i123e");
    Bencode b2 = std::move(b1);
    REQUIRE(b2.root().as_int().value_or(0) == 123);

    // b1 is moved-from, calling operations should not crash
    b1.parse("i456e");
    REQUIRE(b1.root().as_int().value_or(0) == 456);
  }
}

TEST_CASE("Check Node safe accessors and try_get.", "[Node][Accessors]") {
  SECTION("try_get on dictionary entries.") {
    Bencode b("d3:agei28e4:listli1ei2ee4:name5:Alicee");
    const auto &root = b.root();

    const auto *str = root.try_get<String>("name");
    REQUIRE(str != nullptr);
    REQUIRE(str->value() == "Alice");

    const auto *num = root.try_get<Integer>("age");
    REQUIRE(num != nullptr);
    REQUIRE(num->value() == 28);

    const auto *list = root.try_get<List>("list");
    REQUIRE(list != nullptr);
    REQUIRE(list->value().size() == 2);

    REQUIRE(root.try_get<String>("age") == nullptr);
    REQUIRE(root.try_get<Integer>("name") == nullptr);
    REQUIRE(root.try_get<String>("non_existent") == nullptr);
  }

  SECTION("try_get on list elements.") {
    Bencode b("li10ei20e4:teste");
    const auto &root = b.root();

    const auto *first = root.try_get<Integer>(0);
    REQUIRE(first != nullptr);
    REQUIRE(first->value() == 10);

    const auto *second = root.try_get<Integer>(1);
    REQUIRE(second != nullptr);
    REQUIRE(second->value() == 20);

    const auto *third = root.try_get<String>(2);
    REQUIRE(third != nullptr);
    REQUIRE(third->value() == "test");

    REQUIRE(root.try_get<Integer>(2) == nullptr);
    REQUIRE(root.try_get<Integer>(99) == nullptr);
  }

  SECTION("Convenience accessors get_string, get_int, value_or.") {
    Bencode b("d4:ranki1e4:user3:Bobe");
    const auto &root = b.root();

    REQUIRE(root.get_string("user") == "Bob");
    REQUIRE_FALSE(root.get_string("missing").has_value());

    REQUIRE(root.get_int("rank") == 1);
    REQUIRE_FALSE(root.get_int("missing").has_value());

    REQUIRE(root.value_or("user", "guest") == "Bob");
    REQUIRE(root.value_or("non_existent", "guest") == "guest");

    REQUIRE(root.value_or("rank", 999) == 1);
    REQUIRE(root.value_or("missing_int", 999) == 999);
  }

  SECTION("Direct conversions as_string and as_int.") {
    Bencode bStr("5:hello");
    REQUIRE(bStr.root().as_string() == "hello");
    REQUIRE_FALSE(bStr.root().as_int().has_value());

    Bencode bInt("i42e");
    REQUIRE(bInt.root().as_int() == 42);
    REQUIRE_FALSE(bInt.root().as_string().has_value());
  }

  SECTION("Binary data accessors get_binary, binary_or, and as_binary.") {
    const std::string rawBinary("\x00\x01\x02\xFF\x00\xAA", 6);
    const std::string encoded = "d4:hash6:" + rawBinary + "4:name3:Bobe";
    Bencode b(encoded);
    const auto &root = b.root();

    auto binOpt = root.get_binary("hash");
    REQUIRE(binOpt.has_value());
    REQUIRE(binOpt->size() == 6);
    REQUIRE((*binOpt)[0] == std::byte{0x00});
    REQUIRE((*binOpt)[1] == std::byte{0x01});
    REQUIRE((*binOpt)[2] == std::byte{0x02});
    REQUIRE((*binOpt)[3] == std::byte{0xFF});
    REQUIRE((*binOpt)[4] == std::byte{0x00});
    REQUIRE((*binOpt)[5] == std::byte{0xAA});

    REQUIRE_FALSE(root.get_binary("missing").has_value());

    const std::byte fallbackBytes[] = {std::byte{0xDE}, std::byte{0xAD}};
    std::span<const std::byte> fallbackSpan(fallbackBytes);

    auto existingSpan = root.binary_or("hash", fallbackSpan);
    REQUIRE(existingSpan.size() == 6);
    REQUIRE(existingSpan[0] == std::byte{0x00});

    auto missingSpan = root.binary_or("missing", fallbackSpan);
    REQUIRE(missingSpan.size() == 2);
    REQUIRE(missingSpan[0] == std::byte{0xDE});
    REQUIRE(missingSpan[1] == std::byte{0xAD});

    // Test as_binary directly on a string/binary node
    const std::string rawStr("\x00\xAB\x00\xEF", 4);
    Bencode bRaw("4:" + rawStr);
    auto asBin = bRaw.root().as_binary();
    REQUIRE(asBin.has_value());
    REQUIRE(asBin->size() == 4);
    REQUIRE((*asBin)[0] == std::byte{0x00});
    REQUIRE((*asBin)[1] == std::byte{0xAB});
    REQUIRE((*asBin)[2] == std::byte{0x00});
    REQUIRE((*asBin)[3] == std::byte{0xEF});

    // Non-string node should return nullopt for as_binary
    Bencode bInt("i42e");
    REQUIRE_FALSE(bInt.root().as_binary().has_value());
    REQUIRE_FALSE(b.root().as_binary().has_value());
  }
}