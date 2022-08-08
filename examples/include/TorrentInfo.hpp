#pragma once
// ============
// STL includes
// ============
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
// =======
// Bencode
// =======
#include "Bencode.hpp"
#include "Bencode_Types.hpp"
#include "Bencode_Sources.hpp"
//
// =============================
// Torrent file meta information
// =============================
struct TorrentInfo {
  // BencodeLib short cuts
  using Dictionary = BencodeLib::Dictionary;
  using List = BencodeLib::List;
  using String = BencodeLib::String;
  using Integer = BencodeLib::Integer;
  // File details
  struct FileDetails {
    FileDetails(const std::string &path, std::uint64_t length)
        : path(path), length(length) {}
    std::string path;       // Full file path name
    std::uint64_t length{}; // File length in bytes
  };
  // Constructors/destructors
  TorrentInfo() = default;
  TorrentInfo(const std::string &fileName) { load(fileName); }
  TorrentInfo(const TorrentInfo &other) = delete;
  TorrentInfo &operator=(const TorrentInfo &other) = delete;
  TorrentInfo(TorrentInfo &&other) = delete;
  TorrentInfo &operator=(TorrentInfo &&other) = delete;
  ~TorrentInfo() = default;
  // Load torrent file
  void load(const std::string &fileName);
  // Populate torrent info structure from bencoded data
  void get();
  // Return string of torrent info details
  std::string dump();

private:
  std::string getString(const Dictionary &bNode, const char *field) {
    if (bNode.contains(field)) {
      return (BRef<String>(bNode[field]).string());
    }
    return ("");
  }
  std::uint64_t getInteger(const Dictionary &bNode, const char *field) {
    if (bNode.contains(field)) {
      return (BRef<Integer>(bNode[field]).integer());
    }
    return (0);
  }
  std::vector<std::string> getAnnounceList(const Dictionary &bNode) {
    // This is meant to be a simple list of strings but for some reason each
    // string is encased in its own list for an extra level (bug ?).
    if (bNode.contains("announce-list")) {
      std::vector<std::string> servers;
      for (auto &server : BRef<List>(bNode["announce-list"]).list()) {
        for (auto &bNodeString : BRef<List>(server).list()) {
          servers.push_back(BRef<String>(bNodeString).string());
        }
      }
      return (servers);
    }
    return (std::vector<std::string>{});
  }
  std::string getFilePath(const Dictionary &bNode) {
    if (bNode.contains("path")) {
      std::filesystem::path path{};
      for (auto &folder : BRef<List>(bNode["path"]).list()) {
        path /= BRef<String>(folder).string();
      }
      return (path.string());
    }
    return ("");
  }
  std::vector<TorrentInfo::FileDetails> getFilesList(const Dictionary &bNode) {
    if (bNode.contains("files")) {
      std::vector<FileDetails> fileList;
      for (auto &file : BRef<List>(bNode["files"]).list()) {
        fileList.emplace_back(getFilePath(BRef<Dictionary>(file)),
                              getInteger(BRef<Dictionary>(file), "length"));
      }
      return (fileList);
    }
    return (std::vector<FileDetails>{});
  }
  // Main tracker server
  std::string m_announce;
  // Backup tracker server list
  std::vector<std::string> m_announceList;
  // Character encoding used for strings (optional)
  std::string m_encoding;
  // Default file attributes
  std::string m_attribute;
  // Comment
  std::string m_comment;
  // Creation date
  std::uint64_t m_creationDate{};
  // Created by (author)
  std::string m_createdBy;
  // List of torrent files to download
  std::vector<FileDetails> m_files;
  // Length of file to download
  std::uint64_t m_length{};
  // Name of single file to download (root directory in
  // multiple files)
  std::string m_name;
  // Piece length
  std::uint64_t m_pieceLength{};
  // Number of pieces
  std::string m_pieces;
  // Private bitmask
  std::uint64_t m_privateBitMask{};
  // Source of torrent
  std::string m_source;
  // Bencode encoding for torrent file
  const BencodeLib::Bencode m_bEncode;
  // Torrent file name
  std::string m_fileName;
};
// ==============
// PUBLIC METHODS
// ==============
/// <summary>
/// Load torrent file meta information into a structure for processing.
/// </summary>
/// <param name="bNode">Root BNode of decoded torrent file.</param>
inline void TorrentInfo::get() {
  if (m_bEncode.root().getNodeType() != BencodeLib::BNode::Type::dictionary) {
    throw BencodeLib::Error("Valid torrent file not found.");
  }
  auto &bNodeTop = BRef<Dictionary>(m_bEncode.root());
  m_announce = getString(bNodeTop, "announce");
  m_announceList = getAnnounceList(bNodeTop);
  m_encoding = getString(bNodeTop, "encoding");
  m_comment = getString(bNodeTop, "comment");
  m_creationDate = getInteger(bNodeTop, "creation date");
  m_createdBy = getString(bNodeTop, "created by");
  if (bNodeTop.contains("info")) {
    auto &bNodeInfo = BRef<Dictionary>(bNodeTop["info"]);
    m_attribute = getString(bNodeInfo, "attr");
    m_length = getInteger(bNodeInfo, "length");
    m_name = getString(bNodeInfo, "name");
    m_pieceLength = getInteger(bNodeInfo, "piece length");
    m_pieces = getString(bNodeInfo, "pieces");
    m_privateBitMask = getInteger(bNodeInfo, "private");
    m_source = getString(bNodeInfo, "source");
    m_files = getFilesList(bNodeInfo);
  }
}
/// <summary>
/// Display to stdout meta information for torrent file.
/// </summary>
/// <param name="fileName">Torrent file name</param>
/// <param name="info">Meta information</param>
inline std::string TorrentInfo::dump() {
  std::stringstream os;
  os << "\n------------------------------------------------------------\n";
  os << "FILE [ " << m_fileName << " ]\n";
  os << "------------------------------------------------------------\n";
  os << "announce [" << m_announce << "]\n";
  os << "attribute [" << m_attribute << "]\n";
  os << "encoding [" << m_encoding << "]\n";
  os << "comment [" << m_comment << "]\n";
  os << "creation_date [" << m_creationDate << "]\n";
  os << "created_by [" << m_createdBy << "]\n";
  os << "length [" << m_length << "]\n";
  os << "name [" << m_name << "]\n";
  os << "piece length [" << m_pieceLength << "]\n";
  os << "private [" << m_privateBitMask << "]\n";
  os << "source [" << m_source << "]\n";
  for (const auto &file : m_files) {
    os << "path [ " << file.path << "] length [" << file.length << "]\n";
  }
  for (const auto &announceURL : m_announceList) {
    os << "announce url [ " << announceURL << "]\n";
  }
  os << "------------------------------------------------------------";
  return (os.str());
}
/// <summary>
/// Load torrent file into a BNode tree.
/// </summary>
/// <param name="fileName">Torrent file name</param>
inline void TorrentInfo::load(const std::string &fileName) {
  m_fileName = fileName;
  m_bEncode.decode(BencodeLib::FileSource{fileName});
}