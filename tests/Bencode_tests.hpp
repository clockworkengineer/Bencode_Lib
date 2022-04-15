#pragma once
// ======================
// Test framework include
// ======================
#include "catch2/catch_all.hpp"
// =========================
// Class under test includes
// =========================
#include "Bencode.hpp"
#include "BencodeSources.hpp"
#include "BencodeDestinations.hpp"
// ============
// STL includes
// ============
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
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
extern std::string prefixTestDataPath(const std::string &file);
extern bool compareFiles(const std::string &fileName1, const std::string &fileName2);
extern std::string readBencodedBytesFromFile(const std::string &bencodedFileName);