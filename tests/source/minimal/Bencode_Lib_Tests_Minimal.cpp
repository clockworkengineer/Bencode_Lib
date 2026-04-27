#include "catch2/catch_all.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

using namespace Bencode_Lib;

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
