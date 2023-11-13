//
// Program: Convert_Torrent_Files_To_JSON
//
// Description: Use Bencode_Lib to read in torrent file details.
//
// Dependencies: C++20, Bencode_Lib.
//

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "TorrentInfo.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace fs = std::filesystem;

/// <summary>
/// Return a vector of torrent files to analyze.
/// </summary>
/// <returns>Vector of torrent file names</returns>
std::vector<std::string> readTorrentFileList() {
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator((fs::current_path() / "files"))) {
    if (const auto fileName = file.path().string();
        fileName.ends_with(".torrent")) {
      fileList.push_back(fileName);
    }
  }
  return (fileList);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Convert_Torrent_Files_To_JSON.log");
    PLOG_INFO << "Convert_Torrent_Files_To_JSON started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    // For each torrent file extract its information and display
    for (const auto &fileName : readTorrentFileList()) {
      TorrentInfo torrentFile{fileName};
      torrentFile.populate();
      PLOG_INFO << torrentFile.dump();
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Convert_Torrent_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}