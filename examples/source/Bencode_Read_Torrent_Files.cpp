//
// Program: Bencode_Bencode_Read_Torrent_Files
//
// Description: Use Bencode_Lib to read in torrent file details.
// Details on the torrent file format can be found at the wiki page
// https://en.wikipedia.org/wiki/Torrent_file.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "Bencode_TorrentInfo.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    init(plog::debug, "Bencode_Read_Torrent_Files.log");
    PLOG_INFO << "Read_Torrent_File started ...";
    PLOG_INFO << be::Bencode::version();
    // For each torrent file extract its information and display
    for (const auto &fileName : Utility::createTorrentFileList()) {
      TorrentInfo torrentFile{fileName};
      torrentFile.populate();
      PLOG_INFO << torrentFile.dump();
    }
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Bencode_Read_Torrent_Files exited.";
  exit(EXIT_SUCCESS);
}