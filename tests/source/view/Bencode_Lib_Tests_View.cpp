//
// File: Bencode_Lib_Tests_View.cpp
//
// Description: Unit tests for zero-copy non-owning BencodeView and NodeView types.
//

#include "Bencode.hpp"
#include "Bencode_View.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

using namespace Bencode_Lib;
using namespace std::string_literals;

TEST_CASE("BencodeView: Parse scalar integers", "[view][integer]") {
  SECTION("Positive integer") {
    std::string data = "i42e";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_integer());
    REQUIRE(view.root().type() == Bencode_Lib::NodeView::Type::Integer);
    REQUIRE(view.root().as_int().has_value());
    REQUIRE(view.root().as_int().value() == 42);
    REQUIRE_FALSE(view.root().is_string());
    REQUIRE_FALSE(view.root().is_list());
    REQUIRE_FALSE(view.root().is_dict());
  }

  SECTION("Negative integer") {
    std::string data = "i-1337e";
    auto view = Bencode_Lib::Bencode::parseView(data);
    REQUIRE(view.root().as_int().value() == -1337);
  }

  SECTION("Zero integer") {
    std::string data = "i0e";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().as_int().value() == 0);
  }

  SECTION("Large int64_t values") {
    std::string data = "i9223372036854775807e";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().as_int().value() == 9223372036854775807LL);
  }
}

TEST_CASE("BencodeView: Parse scalar strings and zero-copy verification", "[view][string]") {
  SECTION("Simple string without memory allocation") {
    std::string data = "4:spam";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_string());
    REQUIRE(view.root().as_string().has_value());
    REQUIRE(view.root().as_string().value() == "spam");
    REQUIRE(view.root().size() == 4);

    // Verify pointer into source buffer: zero-copy guarantee
    const char *sourceStart = data.data();
    const char *viewStart = view.root().as_string().value().data();
    REQUIRE(viewStart == (sourceStart + 2)); // points directly after '4:'
  }

  SECTION("Empty string") {
    std::string data = "0:";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_string());
    REQUIRE(view.root().as_string().value().empty());
    REQUIRE(view.root().size() == 0);
  }

  SECTION("Binary data with embedded null bytes and high bytes") {
    std::string data = "6:\x00\xFF\x42\x00\xAA\x55"s;
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_string());
    REQUIRE(view.root().size() == 6);

    auto bin = view.root().as_binary();
    REQUIRE(bin.has_value());
    REQUIRE(bin->size() == 6);
    REQUIRE(static_cast<uint8_t>((*bin)[0]) == 0x00);
    REQUIRE(static_cast<uint8_t>((*bin)[1]) == 0xFF);
    REQUIRE(static_cast<uint8_t>((*bin)[2]) == 0x42);
    REQUIRE(static_cast<uint8_t>((*bin)[3]) == 0x00);
    REQUIRE(static_cast<uint8_t>((*bin)[4]) == 0xAA);
    REQUIRE(static_cast<uint8_t>((*bin)[5]) == 0x55);
  }
}

TEST_CASE("BencodeView: Parse lists", "[view][list]") {
  SECTION("Empty list") {
    std::string data = "le";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_list());
    REQUIRE(view.root().size() == 0);
    auto list = view.root().as_list();
    REQUIRE(list.has_value());
    REQUIRE(list->empty());
  }

  SECTION("List of integers and strings") {
    std::string data = "l4:spami42ei-10ee";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_list());
    REQUIRE(view.root().size() == 3);

    auto list = view.root().as_list().value();
    REQUIRE(list[0].is_string());
    REQUIRE(list[0].as_string().value() == "spam");
    REQUIRE(list[1].is_integer());
    REQUIRE(list[1].as_int().value() == 42);
    REQUIRE(list[2].is_integer());
    REQUIRE(list[2].as_int().value() == -10);

    // Operator[] and get(index)
    REQUIRE(view.root()[0].as_string().value() == "spam");
    REQUIRE(view.root()[1].as_int().value() == 42);
    REQUIRE(view.root()[2].as_int().value() == -10);
    REQUIRE_FALSE(view.root().get(3).has_value());
    REQUIRE_THROWS_AS(view.root()[3], std::out_of_range);
  }
}

TEST_CASE("BencodeView: Parse dictionaries", "[view][dict]") {
  SECTION("Empty dictionary") {
    std::string data = "de";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_dict());
    REQUIRE(view.root().size() == 0);
  }

  SECTION("Dictionary with sorted keys and binary lookup") {
    std::string data = "d3:bar4:spam3:fooi42ee";
    auto view = BencodeView::parse(data);
    REQUIRE(view.root().is_dict());
    REQUIRE(view.root().size() == 2);

    // Operator[] on view and root
    REQUIRE(view["bar"].as_string().value() == "spam");
    REQUIRE(view["foo"].as_int().value() == 42);
    REQUIRE(view.root()["bar"].as_string().value() == "spam");
    REQUIRE(view.root()["foo"].as_int().value() == 42);

    // Fast convenience accessors
    REQUIRE(view.root().get_string("bar").value() == "spam");
    REQUIRE(view.root().get_int("foo").value() == 42);
    REQUIRE(view.root().value_or("bar", "default") == "spam");
    REQUIRE(view.root().value_or("missing", "default") == "default");
    REQUIRE(view.root().value_or("foo", 0) == 42);
    REQUIRE(view.root().value_or("missing_int", 99) == 99);

    // Key not found throws in operator[]
    REQUIRE_THROWS_AS(view["nonexistent"], std::out_of_range);

    // Iteration via structured binding
    auto dict = view.root().as_dict().value();
    std::vector<std::string> keys;
    for (const auto &[key, val] : dict) {
      keys.push_back(std::string(key));
    }
    REQUIRE(keys.size() == 2);
    REQUIRE(keys[0] == "bar");
    REQUIRE(keys[1] == "foo");
  }

  SECTION("Nested dictionary and list (torrent-like payload)") {
    std::string data = "d4:infod6:lengthi12345e4:name8:test.txt12:piece lengthi16384ee4:pathl5:dir_a5:dir_bee";
    auto view = Bencode_Lib::Bencode::parseView(data);

    REQUIRE(view.root().is_dict());
    auto info = view["info"];
    REQUIRE(info.is_dict());
    REQUIRE(info["length"].as_int().value() == 12345);
    REQUIRE(info["name"].as_string().value() == "test.txt");
    REQUIRE(info["piece length"].as_int().value() == 16384);

    auto path = view["path"];
    REQUIRE(path.is_list());
    REQUIRE(path.size() == 2);
    REQUIRE(path[0].as_string().value() == "dir_a");
    REQUIRE(path[1].as_string().value() == "dir_b");
  }
}

TEST_CASE("BencodeView: Syntax error handling and validation", "[view][errors]") {
  SECTION("Empty buffer") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::SourceTerminatedEarly);
  }

  SECTION("Trailing bytes") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("i42eextra", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::SourceTerminatedEarly);
  }

  SECTION("Invalid integer with negative zero") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("i-0e", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::NegativeZero);
  }

  SECTION("Invalid integer with leading zeros") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("i042e", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::LeadingZero);
  }

  SECTION("Unterminated integer") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("i42", view);
    REQUIRE_FALSE(status.ok());
  }

  SECTION("Truncated string") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("10:short", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::SourceTerminatedEarly);
  }

  SECTION("Dictionary with unsorted keys") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("d1:b1:x1:a1:ye", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::DictionaryKeyOrder);
  }

  SECTION("Dictionary with duplicate keys") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("d1:a1:x1:a1:ye", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::DuplicateDictionaryKey);
  }

  SECTION("Dictionary with non-string key") {
    Bencode_Lib::BencodeView view;
    auto status = Bencode_Lib::BencodeView::parse("di42e4:spame", view);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::SyntaxError);
  }

  SECTION("Max parser depth exceeded") {
    Bencode_Lib::BencodeView view;
    // Nest 5 lists deep with limit 3
    auto status = Bencode_Lib::BencodeView::parse("lllll5:deepeeeee", view, 3);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == Bencode_Lib::ErrorCode::MaximumParserDepthExceeded);
  }
}
