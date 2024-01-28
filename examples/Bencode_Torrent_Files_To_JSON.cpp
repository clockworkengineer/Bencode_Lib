//
// Program:  Bencode_Torrent_Files_To_JSON
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, Bencode_Lib.
//

#include <stdexcept>

#include "Utility.hpp"
#include "JSON_Encoder.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    Bencode_Lib::Bencode bEncode(std::make_unique<Bencode_Lib::JSON_Encoder>().release());
    // Initialise logging.
    plog::init(plog::debug, "BencodeTorrent_Files_To_JSON.log");
    PLOG_INFO << "BencodeTorrent_Files_To_JSON started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    for (const auto &torrentFileName : Utility::createTorrentFileList()) {
      bEncode.decode(Bencode_Lib::FileSource(torrentFileName));
      bEncode.encode(
          Bencode_Lib::FileDestination(Utility::createFileName(torrentFileName, ".json")));
      PLOG_INFO << "Created file " << Utility::createFileName(torrentFileName, ".json")
                << " from " << torrentFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "BencodeTorrent_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}