#include "Bencode_Lib_Tests.hpp"

TEST_CASE("IDestination (Buffer interface).", "[Bencode][IDestination]") {
  SECTION("Create BufferDestinations.", "[Bencode][IDestination]") {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    REQUIRE_FALSE(!(buffer.size() == 0));
  }
  SECTION("Create BufferDestination and add one character.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.size() == 1);
  }
  SECTION("Create BufferDestination and add an encoded integert.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i65767e");
    REQUIRE(buffer.size() == 7);
    REQUIRE(buffer.toString() == "i65767e");
  }
  SECTION("Add multiple characters one at a time and verify size and content.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add('a');
    buffer.add('b');
    buffer.add('c');
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.toString() == "abc");
  }
  SECTION("Add a std::string and verify size and content.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add(std::string{"hello"});
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.toString() == "hello");
  }
  SECTION("Add a std::string_view and verify size and content.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    std::string_view sv{"bencode"};
    buffer.add(sv);
    REQUIRE(buffer.size() == 7);
    REQUIRE(buffer.toString() == "bencode");
  }
  SECTION("Add multiple strings and verify cumulative size and content.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("li1e");
    buffer.add("i2e");
    buffer.add("i3ee");
    REQUIRE(buffer.size() == 11);
    REQUIRE(buffer.toString() == "li1ei2ei3ee");
  }
  SECTION("Clear a non-empty buffer and verify it is empty.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i42e");
    REQUIRE(buffer.size() == 4);
    buffer.clear();
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.toString().empty());
  }
  SECTION("Clear and then reuse buffer for new content.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i1e");
    buffer.clear();
    buffer.add("i99e");
    REQUIRE(buffer.size() == 4);
    REQUIRE(buffer.toString() == "i99e");
  }
  SECTION("last() returns the most recently added character.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i42e");
    REQUIRE(buffer.last() == 'e');
  }
  SECTION("last() reflects the last character after multiple adds.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("abc");
    buffer.add('z');
    REQUIRE(buffer.last() == 'z');
  }
  SECTION("Stringify a Bencode integer into buffer and verify.",
          "[Bencode][IDestination]") {
    Bencode bencode;
    bencode[0] = 42;
    BufferDestination buffer;
    REQUIRE_NOTHROW(bencode.stringify(buffer));
    REQUIRE(buffer.toString() == "li42ee");
  }
  SECTION("Stringify a Bencode string into buffer and verify.",
          "[Bencode][IDestination]") {
    Bencode bencode;
    bencode["key"] = "value";
    BufferDestination buffer;
    REQUIRE_NOTHROW(bencode.stringify(buffer));
    REQUIRE(buffer.toString() == "d3:key5:valuee");
  }
  SECTION("Stringify twice to same buffer concatenates output.",
          "[Bencode][IDestination]") {
    Bencode bencode;
    bencode[0] = 1;
    BufferDestination buffer;
    bencode.stringify(buffer);
    const std::size_t firstSize = buffer.size();
    bencode.stringify(buffer);
    REQUIRE(buffer.size() == firstSize * 2);
  }
  SECTION("Clear between two stringifies yields only second result.",
          "[Bencode][IDestination]") {
    Bencode bencode;
    bencode[0] = 7;
    BufferDestination buffer;
    bencode.stringify(buffer);
    buffer.clear();
    bencode.stringify(buffer);
    REQUIRE(buffer.toString() == "li7ee");
  }
}