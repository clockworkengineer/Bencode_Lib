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
/// <param name="bNodeDict">Dictionary</param>
/// <param name="field">Field name of string value</param>
/// <returns>String value of field.</returns>
std::string getDictionaryString(const ben::BNodeDict &bNodeDict,
                                const char *field) {
  if (bNodeDict.contains(field)) {
    return (ben::BNodeRef<ben::BNodeString>(bNodeDict[field]).string());
  }
  return ("");
}
/// <summary>
/// Get integer value with named field from a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <param name="field">Field name of integer value</param>
/// <returns>Integer value of field.</returns>
std::uint64_t getDictionaryInteger(const ben::BNodeDict &bNodeDict,
                                   const char *field) {
  if (bNodeDict.contains(field)) {
    return (ben::BNodeRef<ben::BNodeInteger>(bNodeDict[field]).integer());
  }
  return (0);
}
/// <summary>
///  Get vector of announce servers from a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <returns>Vector of announce server names.</returns>
std::vector<std::string> getAnnounceList(const ben::BNodeDict &bNodeDict) {
  // This is meant to be a simple list of strings but for some reason each
  // string is encased in its own list for an extra level (bug ?).
  if (bNodeDict.contains("announce-list")) {
    std::vector<std::string> servers;
    for (auto &bNode :
         ben::BNodeRef<ben::BNodeList>(bNodeDict["announce-list"]).list()) {
      for (auto &bNodeString : ben::BNodeRef<ben::BNodeList>(*bNode).list()) {
        servers.push_back(
            ben::BNodeRef<ben::BNodeString>(*bNodeString).string());
      }
    }
    return (servers);
  }
  return (std::vector<std::string>{});
}
/// <summary>
/// Construct a file path from a list of strings contained in a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <returns>Full file path name.</returns>
std::string getFilePath(const ben::BNodeDict &bNodeDict) {
  if (bNodeDict.contains("path")) {
    std::filesystem::path path{};
    for (auto &folder :
         ben::BNodeRef<ben::BNodeList>(bNodeDict["path"]).list()) {
      path /= ben::BNodeRef<ben::BNodeString>(*folder).string();
    }
    return (path.string());
  }
  return ("");
}
/// <summary>
/// Extract and return a vector of file details from a dictionary.
/// </summary>
/// <param name="bNodeInfoDict">Dictionary</param>
/// <returns>Vector of torrent file details.</returns>
std::vector<TorrentFileDetails>
getFilesList(const ben::BNodeDict &bNodeInfoDict) {
  if (bNodeInfoDict.contains("files")) {
    std::vector<TorrentFileDetails> files;
    for (auto &file :
         ben::BNodeRef<ben::BNodeList>(bNodeInfoDict["files"]).list()) {
      files.emplace_back(
          getFilePath(ben::BNodeRef<ben::BNodeDict>(*file)),
          getDictionaryInteger(ben::BNodeRef<ben::BNodeDict>(*file), "length"));
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
  if (bNode.getNodeType() != ben::BNodeType::dictionary) {
    throw BencodeLib::Error("Valid torrent file not found.");
  }
  auto &bNodeTopLevelDict = ben::BNodeRef<ben::BNodeDict>(bNode);
  info.announce = getDictionaryString(bNodeTopLevelDict, "announce");
  info.announceList = getAnnounceList(bNodeTopLevelDict);
  info.encoding = getDictionaryString(bNodeTopLevelDict, "encoding");
  info.comment = getDictionaryString(bNodeTopLevelDict, "comment");
  info.creationDate = getDictionaryInteger(bNodeTopLevelDict, "creation date");
  info.createdBy = getDictionaryString(bNodeTopLevelDict, "created by");
  if (bNodeTopLevelDict.contains("info")) {
    auto &bNodeInfoDict =
        ben::BNodeRef<ben::BNodeDict>(bNodeTopLevelDict["info"]);
    info.attribute = getDictionaryString(bNodeInfoDict, "attr");
    info.length = getDictionaryInteger(bNodeInfoDict, "length");
    info.name = getDictionaryString(bNodeInfoDict, "name");
    info.pieceLength = getDictionaryInteger(bNodeInfoDict, "piece length");
    info.pieces = getDictionaryString(bNodeInfoDict, "pieces");
    info.privateBitMask = getDictionaryInteger(bNodeInfoDict, "private");
    info.source = getDictionaryString(bNodeInfoDict, "source");
    info.files = getFilesList(bNodeInfoDict);
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
  PLOG_INFO <<  "------------------------------------------------------------";
  PLOG_INFO <<  "FILE [ " << fileName << " ]";
  PLOG_INFO <<  "------------------------------------------------------------";
  PLOG_INFO <<  "announce [" << info.announce << "]";
  PLOG_INFO <<  "attribute [" << info.attribute << "]";
  PLOG_INFO <<  "encoding [" << info.encoding << "]";
  PLOG_INFO <<  "comment [" << info.comment << "]";
  PLOG_INFO <<  "creation_date [" << info.creationDate << "]";
  PLOG_INFO <<  "created_by [" << info.createdBy << "]";
  PLOG_INFO <<  "length [" << info.length << "]";
  PLOG_INFO <<  "name [" << info.name << "]";
  PLOG_INFO <<  "piece length [" << info.pieceLength << "]";
  PLOG_INFO <<  "private [" << info.privateBitMask << "]";
  PLOG_INFO <<  "source [" << info.source << "]";
  for (const auto &file : info.files) {
    PLOG_INFO <<  "path [ " << file.path << "] length [" << file.length << "]";
  }
  for (const auto &announceURL : info.announceList) {
    PLOG_INFO <<  "announce url [ " << announceURL << "]";
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
    ben::Bencode bEncode;
    // Initialise logging.
    plog::init(plog::debug, "Read_Torrent_Files.log");
    PLOG_INFO << "Read_Torrent_File started ...";
    //
    // For each torrent file extract its information and display
    //
    for (const auto &fileName : readTorrentFileList()) {
      bEncode.decode(ben::FileSource{fileName});
      displayTorrentInfo(fileName, getTorrentInfo(bEncode.root()));
    }
  } catch (const std::exception &ex) {
    std::cout <<  "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Read_Torrent_Files exited.";
  exit(EXIT_SUCCESS);
}