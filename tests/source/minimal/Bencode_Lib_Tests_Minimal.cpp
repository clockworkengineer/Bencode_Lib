#include "catch2/catch_all.hpp"

#include <cctype>
#include <memory>
#include <string>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

using namespace Bencode_Lib;

namespace {

class MinimalParser final : public IParser {
protected:
#if BENCODE_ENABLE_EXCEPTIONS
  Node parseImpl(ISource &source) override {
    if (!source.more() || source.current() != 'i') {
      throw Error("Expected integer prefix.");
    }
    source.next();
    int value = 0;
    while (source.more() && std::isdigit(source.current())) {
      value = value * 10 + (source.current() - '0');
      source.next();
    }
    if (!source.more() || source.current() != 'e') {
      throw Error("Expected integer terminator.");
    }
    return Node(value);
  }
#else
  ParseStatus parseImpl(ISource &source, Node &destination) override {
    if (!source.more() || source.current() != 'i') {
      return ParseStatus::failure(ErrorCode::SyntaxError,
                                  "Expected integer prefix.");
    }
    source.next();
    int value = 0;
    while (source.more() && std::isdigit(source.current())) {
      value = value * 10 + (source.current() - '0');
      source.next();
    }
    if (!source.more() || source.current() != 'e') {
      return ParseStatus::failure(ErrorCode::SyntaxError,
                                  "Expected integer terminator.");
    }
    destination = Node(value);
    return ParseStatus::success();
  }
#endif
};

class MinimalStringify final : public IStringify {
public:
  void stringify(const Node &bNode, IDestination &destination) const override {
    destination.add("custom:");
    if (isA<Integer>(bNode)) {
      destination.add(std::to_string(NRef<Integer>(bNode).value()));
    } else {
      destination.add("null");
    }
  }
};

} // namespace

TEST_CASE("Minimal build disables file I/O and optional stringifiers",
          "[Bencode][Minimal]") {
#if defined(BENCODE_ENABLE_FILE_IO) && (BENCODE_ENABLE_FILE_IO == 0)
  SUCCEED("File I/O support is disabled in minimal mode");
#else
  FAIL("Minimal test target must compile with file I/O disabled");
#endif

#if defined(BENCODE_ENABLE_JSON_STRINGIFY) &&                                  \
    (BENCODE_ENABLE_JSON_STRINGIFY == 0)
  SUCCEED("JSON stringify support is disabled in minimal mode");
#else
  FAIL("Minimal test target must compile with JSON stringify support disabled");
#endif

#if defined(BENCODE_ENABLE_XML_STRINGIFY) && (BENCODE_ENABLE_XML_STRINGIFY == 0)
  SUCCEED("XML stringify support is disabled in minimal mode");
#else
  FAIL("Minimal test target must compile with XML stringify support disabled");
#endif

#if defined(BENCODE_ENABLE_YAML_STRINGIFY) &&                                  \
    (BENCODE_ENABLE_YAML_STRINGIFY == 0)
  SUCCEED("YAML stringify support is disabled in minimal mode");
#else
  FAIL("Minimal test target must compile with YAML stringify support disabled");
#endif
}

TEST_CASE("Minimal build supports buffer-based parse/stringify",
          "[Bencode][Minimal]") {
  Bencode bencoder;
#if BENCODE_ENABLE_EXCEPTIONS
  REQUIRE_NOTHROW(bencoder.parse(BufferSource{"i123e"}));
#else
  auto status = bencoder.parse(BufferSource{"i123e"});
  REQUIRE(status.ok());
#endif

  BufferDestination destination;
  REQUIRE_NOTHROW(bencoder.stringify(destination));
  REQUIRE(destination.toString() == "i123e");
}

TEST_CASE("Minimal build file I/O stubs deny file operations safely",
          "[Bencode][Minimal][FileIO][Security]") {
  REQUIRE_THROWS_AS(Bencode::fromFile("dummy.torrent"), Error);
  REQUIRE_THROWS_AS(Bencode::toFile("dummy.torrent", "i1e"), Error);
}

TEST_CASE("Minimal build supports custom parser and custom stringify",
          "[Bencode][Minimal][IParser][IStringify]") {
  auto parser = std::make_unique<MinimalParser>();
  auto stringify = makeStringify<MinimalStringify>();
  Bencode b(std::move(stringify), std::move(parser));
  b.parse(BufferSource{"i77e"});

  BufferDestination destination;
  b.stringify(destination);
  REQUIRE(destination.toString() == "custom:77");
}
