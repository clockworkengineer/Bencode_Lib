#pragma once

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <map>

#include "catch2/catch_all.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"


#include "Bencode_Decoder.hpp"

#define kSingleFileTorrent "singlefile.torrent"
#define kMultiFileTorrent "multifile.torrent"
#define kSingleFileWithErrorTorrent "singlefileerror.torrent"
#define kMultiFileWithErrorTorrent "multifileerror.torrent"
#define kNonExistantTorrent "doesntexist.torrent"
#define kGeneratedTorrentFile "generated.torrent"

std::string prefixTestDataPath(const std::string &file);
bool compareFiles(const std::string &fileName1, const std::string &fileName2);
std::string readBencodedBytesFromFile(const std::string &bencodedFileName);
std::string bufferToString(const std::vector<std::byte> &buffer);