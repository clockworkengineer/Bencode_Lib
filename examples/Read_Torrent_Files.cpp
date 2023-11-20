//
// Program: Read_Torrent_Files
//
// Description: Use Bencode_Lib to read in torrent file details.
// Details on the torrent file format can be found at the wiki page
// https://en.wikipedia.org/wiki/Torrent_file.
//
// Dependencies: C++20, Bencode_Lib.
//

#include <stdexcept>

#include "Utility.hpp"
#include "TorrentInfo.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Read_Torrent_Files.log");
    PLOG_INFO << "Read_Torrent_File started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    // For each torrent file extract its information and display
    for (const auto &fileName : Utility::createTorrentFileList()) {
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