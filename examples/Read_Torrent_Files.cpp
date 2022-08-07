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
      TorrentInfo torrentFile;
      bEncode.decode(ben::FileSource{fileName});
      torrentFile.get(bEncode.root());
      PLOG_INFO << torrentFile.dump(fileName);
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Read_Torrent_Files exited.";
  exit(EXIT_SUCCESS);
}