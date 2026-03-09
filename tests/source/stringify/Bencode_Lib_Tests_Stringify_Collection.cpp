#include "Bencode_Lib_Tests.hpp"

TEST_CASE("Bencode for stringify of collection types (list, dictionary) ",
          "[Bencode][Stringify]") {
  const Bencode bStringify;
  SECTION("Stringify an List of integers('li266ei6780ei88ee').",
          "[Bencode][Stringify][List]") {
    std::string expected{"li266ei6780ei88ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an List of strings "
          "('l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze')."
          "[Bencode][Stringify][List]") {
    std::string expected{
        "l6:sillyy12:poiuytrewqas26:abcdefghijklmnopqrstuvwxyze"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify ampty List."
          "[Bencode][Stringify][List]") {
    std::string expected{"le"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an Dictionary of integers.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d3:onei1e5:threei3e3:twoi2ee"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an Dictionary of strings.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d3:one10:01234567895:three6:qwerty3:two9:asdfghjkle"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify an empty  Dictionary.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"de"};
    BufferSource source(expected);
    BufferDestination destination;
    bStringify.parse(source);
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a single-integer list.", "[Bencode][Stringify][List]") {
    std::string expected{"li42ee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a list containing strings and integers mixed.",
          "[Bencode][Stringify][List]") {
    std::string expected{"li1e3:twoi3e4:foure"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a nested list.", "[Bencode][Stringify][List]") {
    std::string expected{"lli1ei2eeli3ei4eee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a list containing a dictionary.",
          "[Bencode][Stringify][List]") {
    std::string expected{"ld3:keyi99eee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a dictionary with a list value.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d4:numsli1ei2ei3eee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a dictionary with a nested dictionary.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d5:outerd5:inneri42eee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a single-entry dictionary with integer value.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d3:agei30ee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify a single-entry dictionary with string value.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d4:name5:Alicee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify list of empty dictionaries.",
          "[Bencode][Stringify][List]") {
    std::string expected{"ldededee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
  SECTION("Stringify dictionary with multiple list values.",
          "[Bencode][Stringify][Dictionary]") {
    std::string expected{"d1:ali1ei2ee1:bli3ei4eee"};
    BufferDestination destination;
    bStringify.parse(BufferSource{expected});
    bStringify.stringify(destination);
    REQUIRE(destination.toString() == expected);
  }
}