#pragma once

#include "catch2/catch_all.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "JSON_Stringify.hpp"
#include "XML_Stringify.hpp"

#define kSingleFileTorrent "singlefile.torrent"
#define kMultiFileTorrent "multifile.torrent"
#define kSingleFileWithErrorTorrent "singlefileerror.torrent"
#define kMultiFileWithErrorTorrent "multifileerror.torrent"
#define kNonExistantTorrent "doesntexist.torrent"

std::string prefixTestDataPath(const std::string &bencodeFileName);
bool compareFiles(const std::string &fileName1, const std::string &fileName2);
std::string readBencodedBytesFromFile(const std::string &bencodedFileName);
std::string generateRandomFileName(void );

using namespace Bencode_Lib;