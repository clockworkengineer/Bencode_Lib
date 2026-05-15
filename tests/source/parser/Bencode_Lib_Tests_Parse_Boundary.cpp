#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Parse fails fast on max depth exceeded", "[Bencode][Parse][Boundary][FailFast]") {
    Bencode b;
    // Construct a deeply nested list: l l l ... l e e ... e
    std::string deep = std::string(256, 'l') + std::string(256, 'e');
    BufferSource source{deep};
    // Should throw or fail with max depth error
    REQUIRE_THROWS(b.parse(source));
}

TEST_CASE("Parse empty file returns error", "[Bencode][Parse][File][Boundary]") {
    Bencode b;
    // Simulate empty file source
    BufferSource source{std::string{}};
    REQUIRE_THROWS(b.parse(source));
}

TEST_CASE("Parse string at max allowed length", "[Bencode][Parse][String][Boundary]") {
    Bencode b;
    std::string maxstr(Bencode_Lib::String::getMaxStringLength(), 'a');
    std::string encoded = std::to_string(Bencode_Lib::String::getMaxStringLength()) + ":" + maxstr;
    BufferSource source{encoded};
    REQUIRE_NOTHROW(b.parse(source));
}

TEST_CASE("Parse string exceeding max allowed length fails", "[Bencode][Parse][String][Boundary][FailFast]") {
    Bencode b;
    std::string maxstr(Bencode_Lib::String::getMaxStringLength() + 1, 'a');
    std::string encoded = std::to_string(Bencode_Lib::String::getMaxStringLength() + 1) + ":" + maxstr;
    BufferSource source{encoded};
    REQUIRE_THROWS(b.parse(source));
}
