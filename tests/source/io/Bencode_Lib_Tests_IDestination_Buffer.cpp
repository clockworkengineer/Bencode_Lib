#include "Bencode_Lib_Tests.hpp"

TEST_CASE("IDestination (Buffer interface).", "[Bencode][IDestination]") {
  SECTION("Create BufferDestinations.", "[Bencode][IDestination]") {
    REQUIRE_NOTHROW(BufferDestination());
  }
  SECTION("Create BufferDestination and get buffer which should be empty.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    REQUIRE_FALSE(!(buffer.size()==0));
  }
  SECTION("Create BufferDestination and add one character.",
          "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add('i');
    REQUIRE(buffer.size() == 1);
  }
  SECTION(
      "Create BufferDestination and add an encoded integert.",
      "[Bencode][IDestination]") {
    BufferDestination buffer;
    buffer.add("i65767e");
    REQUIRE(buffer.size() == 7);
    REQUIRE(buffer.toString() == "i65767e");
  }
}