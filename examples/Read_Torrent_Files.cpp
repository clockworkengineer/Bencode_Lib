//
// Program: Read_Torrent_Files
//
// Description: Use Bencode_Lib to read in torrent file details.
// Details on the torrent file format can be found at the wiki page
// https://en.wikipedia.org/wiki/Torrent_file.
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

std::vector<std::string> createTorrentFileList() {
  std::vector<std::string> fileList;
  for (auto &file : std::filesystem::directory_iterator((std::filesystem::current_path() / "files"))) {
    if (file.path().extension() == ".torrent") {
      fileList.push_back(file.path().string());
    }
  }
  return (fileList);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Read_Torrent_Files.log");
    PLOG_INFO << "Read_Torrent_File started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    // For each torrent file extract its information and display
    for (const auto &fileName : createTorrentFileList()) {
      TorrentInfo torrentFile{fileName};
      torrentFile.populate();
      PLOG_INFO << torrentFile.dump();
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Read_Torrent_Files exited.";
  exit(EXIT_SUCCESS);
}