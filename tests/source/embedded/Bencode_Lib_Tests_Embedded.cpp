#include "catch2/catch_all.hpp"

#include <cstddef>
#include <span>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "interface/ISaxHandler.hpp"

using namespace Bencode_Lib;

TEST_CASE("Embedded mode compile definitions are active",
          "[Bencode][Embedded]") {
#if defined(BENCODE_EMBEDDED_MODE)
  SUCCEED("Embedded mode is active");
#else
  FAIL("BENCODE_EMBEDDED_MODE must be defined for the embedded test target");
#endif
}

TEST_CASE("Embedded mode enforces fixed-capacity containers",
          "[Bencode][Embedded]") {
#if defined(BENCODE_ENABLE_DYNAMIC_ALLOCATION) &&                              \
    (BENCODE_ENABLE_DYNAMIC_ALLOCATION == 0)
  REQUIRE(BENCODE_MAX_CONTAINER_SIZE > 0);
  List list;
  REQUIRE(list.size() == 0);
  for (int index = 0; index < BENCODE_MAX_CONTAINER_SIZE; ++index) {
    list.add(Node::make<Integer>(index));
  }
  REQUIRE(list.size() == BENCODE_MAX_CONTAINER_SIZE);
  REQUIRE_THROWS_AS(list.add(Node::make<Integer>(999)), std::runtime_error);
#else
  FAIL("Embedded test target must compile with dynamic allocation disabled");
#endif
}

TEST_CASE("Embedded mode disables file I/O support", "[Bencode][Embedded]") {
#if defined(BENCODE_ENABLE_FILE_IO) && (BENCODE_ENABLE_FILE_IO == 0)
  SUCCEED("File I/O support is disabled in embedded mode");
#else
  FAIL("Embedded test target must compile with file I/O disabled");
#endif
}

TEST_CASE("Embedded mode returns parse status instead of throwing",
          "[Bencode][Embedded]") {
#if defined(BENCODE_ENABLE_EXCEPTIONS) && (BENCODE_ENABLE_EXCEPTIONS == 0)
  Bencode bencoder;
  BufferSource source{"i42e"};
  ParseStatus status = bencoder.parse(source);
  REQUIRE(status.ok());
  BufferSource badSource{"i42"};
  ParseStatus badStatus = bencoder.parse(badSource);
  REQUIRE_FALSE(badStatus.ok());
  REQUIRE(badStatus.code == ErrorCode::MissingEndTerminator);
#else
  FAIL("Embedded test target must compile with exception support disabled");
#endif
}

TEST_CASE("Embedded test target defines exception support off",
          "[Bencode][Embedded]") {
#if defined(BENCODE_ENABLE_EXCEPTIONS) && (BENCODE_ENABLE_EXCEPTIONS == 0)
  SUCCEED("Exception support is disabled in embedded mode");
#else
  FAIL("Embedded test target must compile with exception support disabled");
#endif
}

TEST_CASE("Embedded mode enforces nesting depth limits",
          "[Bencode][Embedded]") {
  REQUIRE(BENCODE_MAX_NESTING_DEPTH == 64);
  const auto initialDepth = Bencode::getMaxParserDepth();
  REQUIRE(initialDepth == 64);

  Bencode bencoder;
  Bencode::setMaxParserDepth(3);
  REQUIRE(Bencode::getMaxParserDepth() == 3);

  // 4 nested lists exceeds max parser depth 3
  BufferSource deepSource{"llllleeeee"};
  ParseStatus status = bencoder.parse(deepSource);
  REQUIRE_FALSE(status.ok());
  REQUIRE(status.code == ErrorCode::SyntaxError);

  Bencode::setMaxParserDepth(initialDepth);
}

TEST_CASE("Embedded mode binary span accessors work without allocation",
          "[Bencode][Embedded]") {
  const std::string rawBinary("\x00\xFF\x42", 3);
  const std::string encoded = "d3:dat3:" + rawBinary + "e";
  Bencode b;
  ParseStatus status = b.parse(BufferSource{encoded});
  REQUIRE(status.ok());

  const auto &root = b.root();
  auto binOpt = root.get_binary("dat");
  REQUIRE(binOpt.has_value());
  REQUIRE(binOpt->size() == 3);
  REQUIRE((*binOpt)[0] == std::byte{0x00});
  REQUIRE((*binOpt)[1] == std::byte{0xFF});
  REQUIRE((*binOpt)[2] == std::byte{0x42});

  REQUIRE_FALSE(root.get_binary("nonexistent").has_value());

  const std::byte fallbackBytes[] = {std::byte{0x11}};
  auto fallbackSpan = root.binary_or("nonexistent", fallbackBytes);
  REQUIRE(fallbackSpan.size() == 1);
  REQUIRE(fallbackSpan[0] == std::byte{0x11});

  const std::string rawNodeStr("\xAA\xBB", 2);
  Bencode bRaw;
  REQUIRE(bRaw.parse(BufferSource{"2:" + rawNodeStr}).ok());
  auto asBin = bRaw.root().as_binary();
  REQUIRE(asBin.has_value());
  REQUIRE(asBin->size() == 2);
  REQUIRE((*asBin)[0] == std::byte{0xAA});
  REQUIRE((*asBin)[1] == std::byte{0xBB});
}

TEST_CASE("Embedded mode streaming SAX parser", "[Bencode][Embedded][SAX]") {
  class EmbeddedSaxHandler : public ISaxHandler {
  public:
    int64_t foundInt = 0;
    std::string foundStr;
    bool listStarted = false;
    bool listEnded = false;
    bool dictStarted = false;
    bool dictEnded = false;
    int keyCount = 0;

    bool on_integer(int64_t val) override {
      foundInt = val;
      return true;
    }
    bool on_string(std::string_view sv) override {
      foundStr = std::string(sv);
      return true;
    }
    bool on_list_begin() override { listStarted = true; return true; }
    bool on_list_end() override { listEnded = true; return true; }
    bool on_dictionary_begin() override { dictStarted = true; return true; }
    bool on_dictionary_key(std::string_view) override { keyCount++; return true; }
    bool on_dictionary_end() override { dictEnded = true; return true; }
  };

  EmbeddedSaxHandler handler;
  BufferSource source{"d4:datal4:itemi100eee"};
  ParseStatus status = Bencode::parseSax(source, handler);
  REQUIRE(status.ok());
  REQUIRE(handler.dictStarted);
  REQUIRE(handler.dictEnded);
  REQUIRE(handler.listStarted);
  REQUIRE(handler.listEnded);
  REQUIRE(handler.keyCount == 1);
  REQUIRE(handler.foundInt == 100);
  REQUIRE(handler.foundStr == "item");
}

TEST_CASE("Embedded mode zero-copy BencodeView parser", "[Bencode][Embedded][View]") {
  SECTION("Successful parse and access") {
    BencodeView view;
    std::string_view data = "d3:fooi42e4:listli1ei2eee";
    ParseStatus status = Bencode::parseView(data, view);
    REQUIRE(status.ok());
    REQUIRE(view.root().is_dict());
    REQUIRE(view.root().size() == 2);
    REQUIRE(view.root().get_int("foo").value() == 42);

    auto listNode = view.root().get("list");
    REQUIRE(listNode.has_value());
    REQUIRE(listNode->is_list());
    REQUIRE(listNode->size() == 2);
    REQUIRE(listNode->get(0)->as_int().value() == 1);
    REQUIRE(listNode->get(1)->as_int().value() == 2);
  }

  SECTION("Error reporting without exceptions") {
    BencodeView view;
    std::string_view invalidData = "d3:fooi42"; // truncated
    ParseStatus status = BencodeView::parse(invalidData, view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::MissingEndTerminator);
  }
}


