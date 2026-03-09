#include "Bencode_Lib_Tests.hpp"

TEST_CASE("IDestination (File interface).", "[Bencode][IDestination]") {
  SECTION("Create FileDestination.", "[Bencode][IDestination]") {
    std::string fileName{generateRandomFileName()};
    REQUIRE_NOTHROW(FileDestination(fileName));
    REQUIRE(std::filesystem::exists(fileName));
    std::filesystem::remove(fileName);
  }
  SECTION("Create FileDestination when file already exists.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.close(); // Ensure file is created.
    REQUIRE_NOTHROW(FileDestination(file.getFileName()));
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination and test file exists and should be empty.",
          "[Bencode][IDestination]") {

    FileDestination file{generateRandomFileName()};
    REQUIRE_FALSE(!std::filesystem::exists(file.getFileName()));
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 0);
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination and add one character.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add('i');
    file.close();
    std::filesystem::path filePath{file.getFileName()};
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 1);
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Create FileDestination, add an encoded integer.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i65767e");
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 7);
    std::string expected{readBencodedBytesFromFile(file.getFileName())};
    REQUIRE(expected == "i65767e");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Add a std::string and verify file size and content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add(std::string{"hello"});
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 5);
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "hello");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Add a std::string_view and verify file size and content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    std::string_view sv{"bencode"};
    file.add(sv);
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 7);
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "bencode");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Add multiple strings and verify cumulative file size and content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("li1e");
    file.add("i2e");
    file.add("i3ee");
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 11);
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "li1ei2ei3ee");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("size() tracks bytes written correctly.", "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i42e");
    REQUIRE(file.size() == 4);
    file.add('!');
    REQUIRE(file.size() == 5);
    file.close();
    std::filesystem::remove(file.getFileName());
  }
  SECTION("last() returns the most recently added character.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i42e");
    REQUIRE(file.last() == 'e');
    file.close();
    std::filesystem::remove(file.getFileName());
  }
  SECTION("last() reflects the last character after multiple adds.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("abc");
    file.add('z');
    REQUIRE(file.last() == 'z');
    file.close();
    std::filesystem::remove(file.getFileName());
  }
  SECTION("clear() truncates file and resets size to zero.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i99e");
    REQUIRE(file.size() == 4);
    file.clear();
    REQUIRE(file.size() == 0);
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 0);
    std::filesystem::remove(file.getFileName());
  }
  SECTION("clear() then reuse writes only new content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    file.add("i1e");
    file.clear();
    file.add("i99e");
    file.close();
    REQUIRE(std::filesystem::file_size(file.getFileName()) == 4);
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "i99e");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Stringify a Bencode integer to file and verify content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    Bencode bencode;
    bencode[0] = 42;
    REQUIRE_NOTHROW(bencode.stringify(file));
    file.close();
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "li42ee");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Stringify a Bencode dictionary to file and verify content.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    Bencode bencode;
    bencode["key"] = "value";
    REQUIRE_NOTHROW(bencode.stringify(file));
    file.close();
    REQUIRE(readBencodedBytesFromFile(file.getFileName()) == "d3:key5:valuee");
    std::filesystem::remove(file.getFileName());
  }
  SECTION("Stringify to file then re-parse from file produces identical tree.",
          "[Bencode][IDestination]") {
    FileDestination file{generateRandomFileName()};
    Bencode bencode;
    bencode["name"] = "test";
    bencode["count"] = 7;
    bencode.stringify(file);
    file.close();
    Bencode bencode2;
    REQUIRE_NOTHROW(bencode2.parse(FileSource{file.getFileName()}));
    REQUIRE(NRef<String>(bencode2["name"]).value() == "test");
    REQUIRE(NRef<Integer>(bencode2["count"]).value() == 7);
    std::filesystem::remove(file.getFileName());
  }
}