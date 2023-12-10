#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"


struct TorrentInfo {
  // Bencode_Lib short cuts
  using Dictionary = Bencode_Lib::Dictionary;
  using List = Bencode_Lib::List;
  using String = Bencode_Lib::String;
  using Integer = Bencode_Lib::Integer;
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
  void populate();
  // Return string of torrent info details
  std::string dump();

private:
  std::string getString(const Dictionary &bNode, const char *field) {
    if (bNode.contains(field)) {
      return (BRef<String>(bNode[field]).value());
    }
    return ("");
  }
  std::uint64_t getInteger(const Dictionary &bNode, const char *field) {
    if (bNode.contains(field)) {
      return (BRef<Integer>(bNode[field]).value());
    }
    return (0);
  }
  std::vector<std::string> getAnnounceList(const Dictionary &bNode) {
    // This is meant to be a simple list of strings but for some reason each
    // string is encased in its own list for an extra level (bug ?).
    if (bNode.contains("announce-list")) {
      std::vector<std::string> servers;
      for (auto &server : BRef<List>(bNode["announce-list"]).value()) {
        for (auto &bNodeString : BRef<List>(server).value()) {
          servers.push_back(BRef<String>(bNodeString).value());
        }
      }
      return (servers);
    }
    return (std::vector<std::string>{});
  }
  std::string getFilePath(const Dictionary &bNode) {
    if (bNode.contains("path")) {
      std::filesystem::path path{};
      for (auto &folder : BRef<List>(bNode["path"]).value()) {
        path /= BRef<String>(folder).value();
      }
      return (path.string());
    }
    return ("");
  }
  std::vector<TorrentInfo::FileDetails> getFilesList(const Dictionary &bNode) {
    if (bNode.contains("files")) {
      std::vector<FileDetails> fileList;
      for (auto &file : BRef<List>(bNode["files"]).value()) {
        fileList.emplace_back(getFilePath(BRef<Dictionary>(file)),
                              getInteger(BRef<Dictionary>(file), "length"));
      }
      return (fileList);
    }
    return (std::vector<FileDetails>{});
  }
  // Main tracker server
  std::string announce;
  // Backup tracker server list
  std::vector<std::string> announceList;
  // Character encoding used for strings (optional)
  std::string encoding;
  // Default file attributes
  std::string attribute;
  // Comment
  std::string comment;
  // Creation date
  std::uint64_t creationDate{};
  // Created by (author)
  std::string createdBy;
  // List of torrent files to download
  std::vector<FileDetails> files;
  // Length of file to download
  std::uint64_t length{};
  // Name of single file to download (root directory in
  // multiple files)
  std::string name;
  // Piece length
  std::uint64_t pieceLength{};
  // Number of pieces
  std::string pieces;
  // Private bitmask
  std::uint64_t privateBitMask{};
  // Source of torrent
  std::string source;
  // Bencode encoding for torrent file
  const Bencode_Lib::Bencode bEncode;
  // Torrent file name
  std::string torrentFileName;
};

/// <summary>
/// Load torrent file meta information into a structure for processing.
/// </summary>
/// <param name="bNode">Root BNode of decoded torrent file.</param>
inline void TorrentInfo::populate() {
  if (!bEncode.root().isDictionary()) {
    throw Bencode_Lib::Bencode::Error("Valid torrent file not found.");
  }
  auto &bNodeTop = BRef<Dictionary>(bEncode.root());
  announce = getString(bNodeTop, "announce");
  announceList = getAnnounceList(bNodeTop);
  encoding = getString(bNodeTop, "encoding");
  comment = getString(bNodeTop, "comment");
  creationDate = getInteger(bNodeTop, "creation date");
  createdBy = getString(bNodeTop, "created by");
  if (bNodeTop.contains("info")) {
    auto &bNodeInfo = BRef<Dictionary>(bNodeTop["info"]);
    attribute = getString(bNodeInfo, "attr");
    length = getInteger(bNodeInfo, "length");
    name = getString(bNodeInfo, "name");
    pieceLength = getInteger(bNodeInfo, "piece length");
    pieces = getString(bNodeInfo, "pieces");
    privateBitMask = getInteger(bNodeInfo, "private");
    source = getString(bNodeInfo, "source");
    files = getFilesList(bNodeInfo);
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
  os << "FILE [ " << torrentFileName << " ]\n";
  os << "------------------------------------------------------------\n";
  os << "announce [" << announce << "]\n";
  os << "attribute [" << attribute << "]\n";
  os << "encoding [" << encoding << "]\n";
  os << "comment [" << comment << "]\n";
  os << "creation_date [" << creationDate << "]\n";
  os << "created_by [" << createdBy << "]\n";
  os << "length [" << length << "]\n";
  os << "name [" << name << "]\n";
  os << "piece length [" << pieceLength << "]\n";
  os << "private [" << privateBitMask << "]\n";
  os << "source [" << source << "]\n";
  for (const auto &file : files) {
    os << "path [ " << file.path << "] length [" << file.length << "]\n";
  }
  for (const auto &announceURL : announceList) {
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
  torrentFileName = fileName;
  bEncode.decode(Bencode_Lib::FileSource{torrentFileName});
}