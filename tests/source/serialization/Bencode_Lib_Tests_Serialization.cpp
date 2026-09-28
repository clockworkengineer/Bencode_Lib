//
// File: Bencode_Lib_Tests_Serialization.cpp
//
// Description: Unit tests for C++23 concept-based object mapping and serialization.
//

#include "Bencode.hpp"
#include "Bencode_Serialization.hpp"
#include "Bencode_View.hpp"

#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

using namespace Bencode_Lib;

// Test Struct 1: Simple Person mapped via BENCODE_DEFINE_TYPE_NON_INTRUSIVE
struct Person {
  std::string name;
  int age = 0;
  bool is_active = false;
};
BENCODE_DEFINE_TYPE_NON_INTRUSIVE(Person, name, age, is_active)

// Test Struct 2: TorrentInfo with custom key names mapped via BENCODE_STRUCT
struct TorrentInfo {
  int64_t length = 0;
  std::string name;
  int64_t piece_length = 0;
  std::vector<std::byte> pieces;
};
BENCODE_STRUCT(TorrentInfo,
  (length, "length"),
  (name, "name"),
  (piece_length, "piece length"),
  (pieces, "pieces")
)

// Test Struct 3: Nested Metainfo with optional fields and list of strings
struct TorrentMetainfo {
  std::string announce;
  TorrentInfo info;
  std::optional<std::string> comment;
  std::optional<int64_t> creation_date;
  std::vector<std::string> tags;
};
BENCODE_STRUCT(TorrentMetainfo,
  (announce, "announce"),
  (info, "info"),
  (comment, "comment"),
  (creation_date, "creation date"),
  (tags, "tags")
)

TEST_CASE("Serialization: Primitive types conversion", "[serialization][primitives]") {
  SECTION("Integer types") {
    Node n(42);
    REQUIRE(n.get<int>() == 42);
    REQUIRE(n.get<int64_t>() == 42);
    REQUIRE(n.get<short>() == 42);
    REQUIRE(n.get<uint32_t>() == 42);
  }

  SECTION("Boolean type") {
    Node nTrue(1);
    REQUIRE(nTrue.get<bool>() == true);
    Node nFalse(0);
    REQUIRE(nFalse.get<bool>() == false);

    Node nFromBool = Node::from_object(true);
    REQUIRE(nFromBool.as_int().value() == 1);
  }

  SECTION("String and string_view") {
    Node n("hello world");
    REQUIRE(n.get<std::string>() == "hello world");
    REQUIRE(n.get<std::string_view>() == "hello world");
  }

  SECTION("Binary byte vector") {
    std::vector<std::byte> originalBytes = {std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}};
    Node n = Node::from_object(originalBytes);
    auto decodedBytes = n.get<std::vector<std::byte>>();
    REQUIRE(decodedBytes == originalBytes);
  }
}

TEST_CASE("Serialization: Standard container collections", "[serialization][containers]") {
  SECTION("Vector of integers") {
    std::vector<int> numbers = {1, 2, 3, 5, 8, 13};
    Node n = Node::from_object(numbers);
    REQUIRE(n.get<std::vector<int>>() == numbers);
  }

  SECTION("Vector of strings") {
    std::vector<std::string> words = {"alpha", "beta", "gamma"};
    Node n = Node::from_object(words);
    REQUIRE(n.get<std::vector<std::string>>() == words);
  }

  SECTION("Map of key-value pairs") {
    std::map<std::string, int> scores = {{"alice", 100}, {"bob", 85}, {"charlie", 92}};
    Node n = Node::from_object(scores);
    auto decoded = n.get<std::map<std::string, int>>();
    REQUIRE(decoded == scores);
  }
}

TEST_CASE("Serialization: BENCODE_DEFINE_TYPE_NON_INTRUSIVE struct mapping", "[serialization][struct]") {
  Person p{"Ada Lovelace", 36, true};

  SECTION("Roundtrip through Node DOM") {
    Node n = Node::from_object(p);
    REQUIRE(n["name"].get<std::string>() == "Ada Lovelace");
    REQUIRE(n["age"].get<int>() == 36);
    REQUIRE(n["is_active"].get<bool>() == true);

    Person p2 = n.get<Person>();
    REQUIRE(p2.name == p.name);
    REQUIRE(p2.age == p.age);
    REQUIRE(p2.is_active == p.is_active);
  }

  SECTION("Roundtrip through Bencode string encoding") {
    Bencode b = Bencode::from_object(p);
    std::string encoded = b.encode();

    // Verify canonical Bencode dictionary format with sorted keys
    REQUIRE(encoded == "d3:agei36e9:is_activei1e4:name12:Ada Lovelacee");

    // High-level one-liner parse_object
    Person pDecoded = Bencode::parse_object<Person>(encoded);
    REQUIRE(pDecoded.name == "Ada Lovelace");
    REQUIRE(pDecoded.age == 36);
    REQUIRE(pDecoded.is_active == true);
  }
}

TEST_CASE("Serialization: BENCODE_STRUCT with custom keys and spaces", "[serialization][custom_keys]") {
  TorrentInfo info{
      104857600, // 100 MB
      "linux_distro.iso",
      262144, // 256 KB piece length
      {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}, std::byte{0x04}}};

  Node n = Node::from_object(info);
  REQUIRE(n["length"].get<int64_t>() == 104857600);
  REQUIRE(n["name"].get<std::string>() == "linux_distro.iso");
  REQUIRE(n["piece length"].get<int64_t>() == 262144);
  REQUIRE(n["pieces"].get<std::vector<std::byte>>().size() == 4);

  TorrentInfo decoded = n.get<TorrentInfo>();
  REQUIRE(decoded.length == info.length);
  REQUIRE(decoded.name == info.name);
  REQUIRE(decoded.piece_length == info.piece_length);
  REQUIRE(decoded.pieces == info.pieces);
}

TEST_CASE("Serialization: Nested structs and std::optional handling", "[serialization][nested_optional]") {
  TorrentInfo info{
      5242880,
      "document.pdf",
      65536,
      {std::byte{0xAA}, std::byte{0xBB}}};

  SECTION("Optional fields absent (std::nullopt)") {
    TorrentMetainfo meta{
        "http://tracker.open.org/announce",
        info,
        std::nullopt, // comment omitted
        std::nullopt, // creation date omitted
        {"docs", "pdf", "open"}};

    Bencode b = Bencode::from_object(meta);
    std::string encoded = b.encode();

    // Ensure comment and creation date keys are NOT in the encoded bencode
    REQUIRE(encoded.find("comment") == std::string::npos);
    REQUIRE(encoded.find("creation date") == std::string::npos);

    // Unpack from string using parse_object
    TorrentMetainfo decoded = Bencode::parse_object<TorrentMetainfo>(encoded);
    REQUIRE(decoded.announce == "http://tracker.open.org/announce");
    REQUIRE(decoded.info.name == "document.pdf");
    REQUIRE_FALSE(decoded.comment.has_value());
    REQUIRE_FALSE(decoded.creation_date.has_value());
    REQUIRE(decoded.tags.size() == 3);
    REQUIRE(decoded.tags[0] == "docs");
  }

  SECTION("Optional fields present") {
    TorrentMetainfo meta{
        "http://tracker.open.org/announce",
        info,
        "Official Release",
        1695900000,
        {"release"}};

    Bencode b = Bencode::from_object(meta);
    std::string encoded = b.encode();

    REQUIRE(encoded.find("comment") != std::string::npos);
    REQUIRE(encoded.find("creation date") != std::string::npos);

    TorrentMetainfo decoded = Bencode::parse_object<TorrentMetainfo>(encoded);
    REQUIRE(decoded.comment.has_value());
    REQUIRE(decoded.comment.value() == "Official Release");
    REQUIRE(decoded.creation_date.has_value());
    REQUIRE(decoded.creation_date.value() == 1695900000);
  }
}

TEST_CASE("Serialization: Zero-copy deserialization directly from BencodeView", "[serialization][view]") {
  std::string data = "d8:announce19:http://seed.org/ann4:infod6:lengthi100e4:name6:f1.bin12:piece lengthi16e6:pieces2:\x11\x22" "e4:tagsl4:demoee";

  BencodeView view = BencodeView::parse(data);
  TorrentMetainfo meta = view.get<TorrentMetainfo>();

  REQUIRE(meta.announce == "http://seed.org/ann");
  REQUIRE(meta.info.name == "f1.bin");
  REQUIRE(meta.info.length == 100);
  REQUIRE(meta.info.piece_length == 16);
  REQUIRE(meta.info.pieces.size() == 2);
  REQUIRE(meta.tags.size() == 1);
  REQUIRE(meta.tags[0] == "demo");
  REQUIRE_FALSE(meta.comment.has_value());
}

TEST_CASE("Serialization: Error handling for missing required keys", "[serialization][errors]") {
  // Missing required "name" key in Person
  std::string invalidData = "d3:agei25e9:is_activei1ee";
  BencodeView view = BencodeView::parse(invalidData);

  REQUIRE_THROWS_AS(view.get<Person>(), Node::Error);
}
