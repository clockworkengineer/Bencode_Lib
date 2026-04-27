#include "Bencode_Lib_Tests.hpp"
#include <chrono>
#include <random>

/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="bencodeFileName">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &bencodeFileName) {
  if (std::filesystem::is_directory("./files")) {
    return (std::filesystem::current_path() / "./files" / bencodeFileName)
        .string();
  } else {
    return (std::filesystem::current_path() / "../files" / bencodeFileName)
        .string();
  }
}
/// <summary>
/// Compare two Bencoded files.
/// </summary>
/// <param name="fileName1">Bencoded file 1</param>
/// <param name="fileName2">Bencoded file 2</param>
/// <returns>true if files the same, false otherwise</returns>
bool compareFiles(const std::string &fileName1, const std::string &fileName2) {
  std::ifstream file1(fileName1, std::ifstream::binary | std::ifstream::ate);
  std::ifstream file2(fileName2, std::ifstream::binary | std::ifstream::ate);
  if (file1.fail() || file2.fail()) {
    return false; // file problem
  }
  if (file1.tellg() != file2.tellg()) {
    return false; // size mismatch
  }
  // seek back to beginning and use std::equal to compare contents
  file1.seekg(0, std::ifstream::beg);
  file2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator(file1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator(file2.rdbuf()));
}
/// <summary>
/// Open a Bencoded file, read its contents into a string buffer and return
/// the buffer.
/// </summary>
/// <param name="bencodedFileName">Bencoded file name</param>
/// <returns></returns>
std::string readBencodedBytesFromFile(const std::string &bencodedFileName) {
  std::ifstream bencodedFile;
  bencodedFile.open(bencodedFileName, std::ifstream::binary);
  std::ostringstream bencodedFileBuffer;
  bencodedFileBuffer << bencodedFile.rdbuf();
  bencodedFile.close();
  return bencodedFileBuffer.str();
}
/// <summary>
/// Generate unique file name.
/// </summary>
/// <returns>Unique torrent file name</returns>
std::string generateRandomFileName(void) {
  const auto tempDir = std::filesystem::temp_directory_path();
  std::mt19937_64 rng(
      static_cast<unsigned long long>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count()));
  std::uniform_int_distribution<unsigned long long> dist;
  std::filesystem::path namepath;
  do {
    namepath = tempDir / ("bencode-test-" + std::to_string(dist(rng)) + ".tmp");
  } while (std::filesystem::exists(namepath));
  return namepath.string();
}

TEST_CASE("Check helper functions.", "[Bencode][Helpers]") {
  SECTION("prefixTestDataPath returns a non-empty path.",
          "[Bencode][Helpers][prefixTestDataPath]") {
    const std::string path{prefixTestDataPath(kSingleFileTorrent)};
    REQUIRE_FALSE(path.empty());
  }
  SECTION("prefixTestDataPath result ends with the file name.",
          "[Bencode][Helpers][prefixTestDataPath]") {
    const std::string path{prefixTestDataPath(kSingleFileTorrent)};
    REQUIRE(path.ends_with(kSingleFileTorrent));
  }
  SECTION("prefixTestDataPath points to a file that exists.",
          "[Bencode][Helpers][prefixTestDataPath]") {
    const std::string path{prefixTestDataPath(kSingleFileTorrent)};
    REQUIRE(std::filesystem::exists(path));
  }
  SECTION("compareFiles returns true for identical files.",
          "[Bencode][Helpers][compareFiles]") {
    const std::string file1{generateRandomFileName()};
    const std::string file2{generateRandomFileName()};
    const std::string content{"d3:keyi42ee"};
    Bencode::toFile(file1, content);
    Bencode::toFile(file2, content);
    REQUIRE(compareFiles(file1, file2));
    std::filesystem::remove(file1);
    std::filesystem::remove(file2);
  }
  SECTION("compareFiles returns false for files with different content.",
          "[Bencode][Helpers][compareFiles]") {
    const std::string file1{generateRandomFileName()};
    const std::string file2{generateRandomFileName()};
    Bencode::toFile(file1, "i1e");
    Bencode::toFile(file2, "i2e");
    REQUIRE_FALSE(compareFiles(file1, file2));
    std::filesystem::remove(file1);
    std::filesystem::remove(file2);
  }
  SECTION("compareFiles returns false for files with different sizes.",
          "[Bencode][Helpers][compareFiles]") {
    const std::string file1{generateRandomFileName()};
    const std::string file2{generateRandomFileName()};
    Bencode::toFile(file1, "i1e");
    Bencode::toFile(file2, "li1ei2ee");
    REQUIRE_FALSE(compareFiles(file1, file2));
    std::filesystem::remove(file1);
    std::filesystem::remove(file2);
  }
  SECTION("compareFiles returns false if a file does not exist.",
          "[Bencode][Helpers][compareFiles]") {
    const std::string file1{generateRandomFileName()};
    Bencode::toFile(file1, "i1e");
    REQUIRE_FALSE(compareFiles(file1, "/does/not/exist.ben"));
    std::filesystem::remove(file1);
  }
  SECTION("readBencodedBytesFromFile returns the correct content.",
          "[Bencode][Helpers][readBencodedBytesFromFile]") {
    const std::string file{generateRandomFileName()};
    const std::string content{"d4:name5:Nielse"};
    Bencode::toFile(file, content);
    REQUIRE(readBencodedBytesFromFile(file) == content);
    std::filesystem::remove(file);
  }
  SECTION("readBencodedBytesFromFile returns empty string for empty file.",
          "[Bencode][Helpers][readBencodedBytesFromFile]") {
    const std::string file{generateRandomFileName()};
    Bencode::toFile(file, "");
    REQUIRE(readBencodedBytesFromFile(file).empty());
    std::filesystem::remove(file);
  }
  SECTION("readBencodedBytesFromFile reads real torrent file bytes.",
          "[Bencode][Helpers][readBencodedBytesFromFile]") {
    const std::string content{
        readBencodedBytesFromFile(prefixTestDataPath(kSingleFileTorrent))};
    REQUIRE_FALSE(content.empty());
    REQUIRE(content.front() == 'd');
  }
  SECTION("generateRandomFileName returns a non-empty path.",
          "[Bencode][Helpers][generateRandomFileName]") {
    REQUIRE_FALSE(generateRandomFileName().empty());
  }
  SECTION("generateRandomFileName returns unique names on each call.",
          "[Bencode][Helpers][generateRandomFileName]") {
    REQUIRE(generateRandomFileName() != generateRandomFileName());
  }
  SECTION("generateRandomFileName result is inside the temp directory.",
          "[Bencode][Helpers][generateRandomFileName]") {
    const std::string name{generateRandomFileName()};
    const std::string tempDir{std::filesystem::temp_directory_path().string()};
    REQUIRE(name.starts_with(tempDir));
  }
}