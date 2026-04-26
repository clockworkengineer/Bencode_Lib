#include "catch2/catch_all.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

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
