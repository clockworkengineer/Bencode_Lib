// File: Bencode_Lib_Tests_Parse_Sax.cpp
//
// Description: Unit tests for the streaming event-driven SAX parser.
//

#include "Bencode_Lib_Tests.hpp"
#include "interface/ISaxHandler.hpp"
#include "implementation/parser/Sax_Parser.hpp"

#include <string>
#include <vector>

namespace {

// Test handler that records events in chronological order
class EventRecordingSaxHandler : public ISaxHandler {
public:
  std::vector<std::string> events;
  int64_t lastInteger = 0;
  std::string lastString;
  std::string lastKey;
  std::string abortOnKey;
  int64_t abortOnInteger = -999999;

  bool on_integer(int64_t value) override {
    lastInteger = value;
    events.push_back("int:" + std::to_string(value));
    if (value == abortOnInteger) {
      return false;
    }
    return true;
  }

  bool on_string(std::string_view value) override {
    lastString = std::string(value);
    events.push_back("str:" + std::string(value));
    return true;
  }

  bool on_list_begin() override {
    events.push_back("list_begin");
    return true;
  }

  bool on_list_end() override {
    events.push_back("list_end");
    return true;
  }

  bool on_dictionary_begin() override {
    events.push_back("dict_begin");
    return true;
  }

  bool on_dictionary_key(std::string_view key) override {
    lastKey = std::string(key);
    events.push_back("key:" + std::string(key));
    if (!abortOnKey.empty() && key == abortOnKey) {
      return false;
    }
    return true;
  }

  bool on_dictionary_end() override {
    events.push_back("dict_end");
    return true;
  }
};

} // namespace

TEST_CASE("Streaming SAX parser parses scalar values", "[Bencode][SAX]") {
  SECTION("Parse positive integer.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("i42e", handler));
    REQUIRE(handler.events.size() == 1);
    REQUIRE(handler.events[0] == "int:42");
    REQUIRE(handler.lastInteger == 42);
  }

  SECTION("Parse negative integer.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("i-100e", handler));
    REQUIRE(handler.events.size() == 1);
    REQUIRE(handler.events[0] == "int:-100");
  }

  SECTION("Parse zero.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("i0e", handler));
    REQUIRE(handler.events.size() == 1);
    REQUIRE(handler.events[0] == "int:0");
  }

  SECTION("Parse simple string.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("5:hello", handler));
    REQUIRE(handler.events.size() == 1);
    REQUIRE(handler.events[0] == "str:hello");
    REQUIRE(handler.lastString == "hello");
  }

  SECTION("Parse empty string.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("0:", handler));
    REQUIRE(handler.events.size() == 1);
    REQUIRE(handler.events[0] == "str:");
    REQUIRE(handler.lastString.empty());
  }

  SECTION("Parse string with raw bytes and null terminator.") {
    EventRecordingSaxHandler handler;
    const std::string rawBinary("\x00\xFF\x00\x42", 4);
    REQUIRE(Bencode::parseSax("4:" + rawBinary, handler));
    REQUIRE(handler.lastString.size() == 4);
    REQUIRE(handler.lastString == rawBinary);
  }
}

TEST_CASE("Streaming SAX parser parses list collections", "[Bencode][SAX]") {
  SECTION("Parse empty list.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("le", handler));
    REQUIRE(handler.events == std::vector<std::string>{"list_begin", "list_end"});
  }

  SECTION("Parse list with integers and strings.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("li1ei2e5:threee", handler));
    REQUIRE(handler.events == std::vector<std::string>{
        "list_begin", "int:1", "int:2", "str:three", "list_end"
    });
  }

  SECTION("Parse nested lists.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("lli1eeli2eee", handler));
    REQUIRE(handler.events == std::vector<std::string>{
        "list_begin", "list_begin", "int:1", "list_end",
        "list_begin", "int:2", "list_end", "list_end"
    });
  }
}

TEST_CASE("Streaming SAX parser parses dictionary collections", "[Bencode][SAX]") {
  SECTION("Parse empty dictionary.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("de", handler));
    REQUIRE(handler.events == std::vector<std::string>{"dict_begin", "dict_end"});
  }

  SECTION("Parse dictionary with multiple sorted keys.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("d3:agei30e4:name3:Bobe", handler));
    REQUIRE(handler.events == std::vector<std::string>{
        "dict_begin", "key:age", "int:30", "key:name", "str:Bob", "dict_end"
    });
  }

  SECTION("Parse nested dictionary and list.") {
    EventRecordingSaxHandler handler;
    REQUIRE(Bencode::parseSax("d4:infod4:sizei2048ee4:tagsl4:testee", handler));
    REQUIRE(handler.events == std::vector<std::string>{
        "dict_begin",
        "key:info", "dict_begin", "key:size", "int:2048", "dict_end",
        "key:tags", "list_begin", "str:test", "list_end",
        "dict_end"
    });
  }
}

TEST_CASE("Streaming SAX parser supports early termination", "[Bencode][SAX]") {
  SECTION("Early abort when specific key is found.") {
    EventRecordingSaxHandler handler;
    handler.abortOnKey = "target";

    // "alpha" -> "target" (abort here) -> "zeta" (should never be reached)
    const std::string input = "d5:alphai1e6:targeti2e4:zetai3ee";
    bool completed = Bencode::parseSax(input, handler);

    REQUIRE_FALSE(completed);
    REQUIRE(handler.events == std::vector<std::string>{
        "dict_begin", "key:alpha", "int:1", "key:target"
    });
    // Check that zeta was never parsed
    for (const auto &ev : handler.events) {
      REQUIRE(ev != "key:zeta");
    }
  }

  SECTION("Early abort on specific integer value.") {
    EventRecordingSaxHandler handler;
    handler.abortOnInteger = 42;

    const std::string input = "li10ei20ei42ei99ee";
    bool completed = Bencode::parseSax(input, handler);

    REQUIRE_FALSE(completed);
    REQUIRE(handler.events == std::vector<std::string>{
        "list_begin", "int:10", "int:20", "int:42"
    });
  }

  SECTION("SaxParser::parse returns UserAborted status on early exit.") {
    EventRecordingSaxHandler handler;
    handler.abortOnKey = "stop";

    ParseStatus status = SaxParser::parse("d4:stopi0e4:nexti1ee", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::UserAborted);
  }
}

TEST_CASE("Streaming SAX parser rejects malformed Bencode inputs", "[Bencode][SAX]") {
  EventRecordingSaxHandler handler;

  SECTION("Unsorted dictionary keys.") {
    ParseStatus status = SaxParser::parse("d4:zeta1:a5:alpha1:be", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::DictionaryKeyOrder);
  }

  SECTION("Duplicate dictionary key.") {
    ParseStatus status = SaxParser::parse("d3:agei20e3:agei30ee", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::DuplicateDictionaryKey);
  }

  SECTION("Negative zero integer.") {
    ParseStatus status = SaxParser::parse("i-0e", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::NegativeZero);
  }

  SECTION("Leading zero in integer.") {
    ParseStatus status = SaxParser::parse("i05e", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::LeadingZero);
  }

  SECTION("Missing colon in string length.") {
    ParseStatus status = SaxParser::parse("5hello", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::MissingColon);
  }

  SECTION("Unexpected trailing data after payload.") {
    ParseStatus status = SaxParser::parse("i42etrail", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::SourceTerminatedEarly);
  }

  SECTION("Empty buffer returns failure.") {
    ParseStatus status = SaxParser::parse("", handler);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::SourceTerminatedEarly);
  }

  SECTION("Nesting depth limit enforcement.") {
    // 5 nested lists with maxDepth = 4
    ParseStatus status = SaxParser::parse("llllleeeee", handler, 4);
    REQUIRE_FALSE(status.ok());
    REQUIRE(status.code == ErrorCode::MaximumParserDepthExceeded);
  }

  SECTION("In exception mode, malformed syntax throws IParser::Error.") {
    REQUIRE_THROWS_AS(Bencode::parseSax("i-0e", handler), IParser::Error);
    REQUIRE_THROWS_AS(Bencode::parseSax("d4:zeta1:a5:alpha1:be", handler), IParser::Error);
  }
}

TEST_CASE("Streaming SAX parser streams torrent file with O(1) heap overhead", "[Bencode][SAX]") {
  class TorrentInspectorHandler : public ISaxHandler {
  public:
    std::string announce;
    int64_t pieceLength = 0;
    std::size_t totalPiecesBytes = 0;
    std::string currentKey;
    int keyCount = 0;

    bool on_integer(int64_t value) override {
      if (currentKey == "piece length") {
        pieceLength = value;
      }
      return true;
    }

    bool on_string(std::string_view value) override {
      if (currentKey == "announce") {
        announce = std::string(value);
      } else if (currentKey == "pieces") {
        totalPiecesBytes = value.size();
      }
      return true;
    }

    bool on_list_begin() override { return true; }
    bool on_list_end() override { return true; }
    bool on_dictionary_begin() override { return true; }

    bool on_dictionary_key(std::string_view key) override {
      currentKey = std::string(key);
      keyCount++;
      return true;
    }

    bool on_dictionary_end() override { return true; }
  };

  TorrentInspectorHandler handler;
  FileSource source{prefixTestDataPath(kSingleFileTorrent)};
  bool completed = Bencode::parseSax(source, handler);

  REQUIRE(completed);
  REQUIRE_FALSE(handler.announce.empty());
  REQUIRE(handler.pieceLength > 0);
  REQUIRE(handler.totalPiecesBytes > 0);
  REQUIRE(handler.totalPiecesBytes % 20 == 0); // BitTorrent SHA-1 hashes are 20 bytes each
  REQUIRE(handler.keyCount > 0);
}
