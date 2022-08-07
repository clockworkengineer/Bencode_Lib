//
// Program: Read_Torrent_Files
//
// Description: Use BencodeLib to read in torrent file details.
// Details on the torrent file format can be found at the wiki page
// https://en.wikipedia.org/wiki/Torrent_file.
//
// Dependencies: C20++, BencodeLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>
// =======
// Bencode
// =======
#include "Bencode.hpp"
#include "Bencode_Types.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"
//
// Torrent includes
//
#include "TorrentInfo.hpp"
// =======
// Logging
// =======
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
// ==========
// NAMESPACES
// ==========
namespace ben = BencodeLib;
namespace fs = std::filesystem;
// ======================
// LOCAL TYPES/DEFINITIONS
// ======================
/// <summary>
/// Get string value with named field from a dictionary.
/// </summary>
/// <param name="bNodeDictionary">Dictionary</param>
/// <param name="field">Field name of string value</param>
/// <returns>String value of field.</returns>
std::string getDictionaryString(const ben::Dictionary &bNodeDictionary,
                                const char *field) {
  if (bNodeDictionary.contains(field)) {
    return (ben::BRef<ben::String>(bNodeDictionary[field]).string());
  }
  return ("");
}
/// <summary>
/// Get integer value with named field from a dictionary.
/// </summary>
/// <param name="bNodeDictionary">Dictionary</param>
/// <param name="field">Field name of integer value</param>
/// <returns>Integer value of field.</returns>
std::uint64_t getDictionaryInteger(const ben::Dictionary &bNodeDictionary,
                                   const char *field) {
  if (bNodeDictionary.contains(field)) {
    return (ben::BRef<ben::Integer>(bNodeDictionary[field]).integer());
  }
  return (0);
}
/// <summary>
///  Get vector of announce servers from a dictionary.
/// </summary>
/// <param name="bNodeDictionary">Dictionary</param>
/// <returns>Vector of announce server names.</returns>
std::vector<std::string> getAnnounceList(const ben::Dictionary &bNodeDictionary) {
  // This is meant to be a simple list of strings but for some reason each
  // string is encased in its own list for an extra level (bug ?).
  if (bNodeDictionary.contains("announce-list")) {
    std::vector<std::string> servers;
    for (auto &bNode :
         ben::BRef<ben::List>(bNodeDictionary["announce-list"]).list()) {
      for (auto &bNodeString : ben::BRef<ben::List>(bNode).list()) {
        servers.push_back(ben::BRef<ben::String>(bNodeString).string());
      }
    }
    return (servers);
  }
  return (std::vector<std::string>{});
}
/// <summary>
/// Construct a file path from a list of strings contained in a dictionary.
/// </summary>
/// <param name="bNodeDictionary">Dictionary</param>
/// <returns>Full file path name.</returns>
std::string getFilePath(const ben::Dictionary &bNodeDictionary) {
  if (bNodeDictionary.contains("path")) {
    std::filesystem::path path{};
    for (auto &folder : ben::BRef<ben::List>(bNodeDictionary["path"]).list()) {
      path /= ben::BRef<ben::String>(folder).string();
    }
    return (path.string());
  }
  return ("");
}
/// <summary>
/// Extract and return a vector of file details from a dictionary.
/// </summary>
/// <param name="bNodeInfoDictionary">Dictionary</param>
/// <returns>Vector of torrent file details.</returns>
std::vector<TorrentFileDetails>
getFilesList(const ben::Dictionary &bNodeInfoDictionary) {
  if (bNodeInfoDictionary.contains("files")) {
    std::vector<TorrentFileDetails> files;
    for (auto &file : ben::BRef<ben::List>(bNodeInfoDictionary["files"]).list()) {
      files.emplace_back(
          getFilePath(ben::BRef<ben::Dictionary>(file)),
          getDictionaryInteger(ben::BRef<ben::Dictionary>(file), "length"));
    }
    return (files);
  }
  return (std::vector<TorrentFileDetails>{});
}
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Load torrent file meta information into a structure for processing.
/// </summary>
/// <param name="bNode">Root BNode of decoded torrent file.</param>
/// <returns>Torrent file meta information structure.</returns>
TorrentMetaInfo getTorrentInfo(const ben::BNode &bNode) {
  TorrentMetaInfo info;
  if (bNode.getNodeType() != ben::BNode::Type::dictionary) {
    throw BencodeLib::Error("Valid torrent file not found.");
  }
  auto &bNodeTopLevelDictionary = ben::BRef<ben::Dictionary>(bNode);
  info.announce = getDictionaryString(bNodeTopLevelDictionary, "announce");
  info.announceList = getAnnounceList(bNodeTopLevelDictionary);
  info.encoding = getDictionaryString(bNodeTopLevelDictionary, "encoding");
  info.comment = getDictionaryString(bNodeTopLevelDictionary, "comment");
  info.creationDate = getDictionaryInteger(bNodeTopLevelDictionary, "creation date");
  info.createdBy = getDictionaryString(bNodeTopLevelDictionary, "created by");
  if (bNodeTopLevelDictionary.contains("info")) {
    auto &bNodeInfoDictionary = ben::BRef<ben::Dictionary>(bNodeTopLevelDictionary["info"]);
    info.attribute = getDictionaryString(bNodeInfoDictionary, "attr");
    info.length = getDictionaryInteger(bNodeInfoDictionary, "length");
    info.name = getDictionaryString(bNodeInfoDictionary, "name");
    info.pieceLength = getDictionaryInteger(bNodeInfoDictionary, "piece length");
    info.pieces = getDictionaryString(bNodeInfoDictionary, "pieces");
    info.privateBitMask = getDictionaryInteger(bNodeInfoDictionary, "private");
    info.source = getDictionaryString(bNodeInfoDictionary, "source");
    info.files = getFilesList(bNodeInfoDictionary);
  }
  return (info);
}
/// <summary>
/// Display to stdout meta information for torrent file.
/// </summary>
/// <param name="fileName">Torrent file name</param>
/// <param name="info">Meta information</param>
void displayTorrentInfo(const std::string &fileName,
                        const TorrentMetaInfo &info) {
  PLOG_INFO << "------------------------------------------------------------";
  PLOG_INFO << "FILE [ " << fileName << " ]";
  PLOG_INFO << "------------------------------------------------------------";
  PLOG_INFO << "announce [" << info.announce << "]";
  PLOG_INFO << "attribute [" << info.attribute << "]";
  PLOG_INFO << "encoding [" << info.encoding << "]";
  PLOG_INFO << "comment [" << info.comment << "]";
  PLOG_INFO << "creation_date [" << info.creationDate << "]";
  PLOG_INFO << "created_by [" << info.createdBy << "]";
  PLOG_INFO << "length [" << info.length << "]";
  PLOG_INFO << "name [" << info.name << "]";
  PLOG_INFO << "piece length [" << info.pieceLength << "]";
  PLOG_INFO << "private [" << info.privateBitMask << "]";
  PLOG_INFO << "source [" << info.source << "]";
  for (const auto &file : info.files) {
    PLOG_INFO << "path [ " << file.path << "] length [" << file.length << "]";
  }
  for (const auto &announceURL : info.announceList) {
    PLOG_INFO << "announce url [ " << announceURL << "]";
  }
}
/// <summary>
/// Return directory name containing torrent files.
/// </summary>
/// <returns>Torrent file directory</returns>
std::string torrentFileDirectory() {
  return ((fs::current_path() / "files").string());
}
/// <summary>
/// Return a vector of torrent files to analyze.
/// </summary>
/// <returns>Vector of torrent file names</returns>
std::vector<std::string> readTorrentFileList() {
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator(fs::path(torrentFileDirectory()))) {
    if (const auto fileName = file.path().string();
        fileName.ends_with(".torrent")) {
      fileList.push_back(fileName);
    }
  }
  return (fileList);
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    const ben::Bencode bEncode;
    // Initialise logging.
    plog::init(plog::debug, "Read_Torrent_Files.log");
    PLOG_INFO << "Read_Torrent_File started ...";
    PLOG_INFO << ben::Bencode().version();
    //
    // For each torrent file extract its information and display
    //
    for (const auto &fileName : readTorrentFileList()) {
      bEncode.decode(ben::FileSource{fileName});
      displayTorrentInfo(fileName, getTorrentInfo(bEncode.root()));
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Read_Torrent_Files exited.";
  exit(EXIT_SUCCESS);
}