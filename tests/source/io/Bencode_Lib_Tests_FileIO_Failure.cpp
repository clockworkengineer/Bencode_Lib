#include "Bencode_Lib_Tests.hpp"
#include <filesystem>

TEST_CASE("Bencode::fromFile throws on missing file", "[Bencode][IO][Error]") {
  const std::string missingFile = prefixTestDataPath(kNonExistantTorrent);
  REQUIRE_THROWS_AS(Bencode::fromFile(missingFile), Error);
  REQUIRE_THROWS_WITH(
      Bencode::fromFile(missingFile),
      "Bencode Error: Bencode file input stream failed to open or does not exist.");
}

TEST_CASE("Bencode::toFile throws when the target path is a directory",
          "[Bencode][IO][Error]") {
  const std::string directoryPath = std::filesystem::temp_directory_path().string();
  REQUIRE_THROWS_AS(Bencode::toFile(directoryPath, "i42e"), Error);
  REQUIRE_THROWS_WITH(
      Bencode::toFile(directoryPath, "i42e"),
      "Bencode Error: Bencode file output stream failed to open or could not be created.");
}
