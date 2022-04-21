#pragma once
// ============
// STL includes
// ============
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
// ======================
// Test framework include
// ======================
#include "catch2/catch.hpp"
// =========================
// Class under test includes
// =========================
#include "Bencode.hpp"
#include "BencodeSources.hpp"
#include "BencodeDestinations.hpp"
// ===================
// Unit test constants
// ===================
#define kSingleFileTorrent "singlefile.torrent"
#define kMultiFileTorrent "multifile.torrent"
#define kSingleFileWithErrorTorrent  "singlefileerror.torrent"
#define kMultiFileWithErrorTorrent  "multifileerror.torrent"
#define kNonExistantTorrent  "doesntexist.torrent"
#define kGeneratedTorrentFile  "generated.torrent"
// ==========================
// Unit test helper functions
// ==========================
std::string prefixTestDataPath(const std::string &file);
bool compareFiles(const std::string &fileName1, const std::string &fileName2);
std::string readBencodedBytesFromFile(const std::string &bencodedFileName);
std::string bufferToString(const std::vector<std::byte> &buffer);