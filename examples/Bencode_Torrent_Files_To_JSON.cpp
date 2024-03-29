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

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    be::Bencode bEncode(
        std::make_unique<be::JSON_Encoder>(be::JSON_Translator()).release());
    // Initialise logging.
    plog::init(plog::debug, "BencodeTorrent_Files_To_JSON.log");
    PLOG_INFO << "BencodeTorrent_Files_To_JSON started ...";
    PLOG_INFO << be::Bencode().version();
    for (const auto &torrentFileName : Utility::createTorrentFileList()) {
      bEncode.decode(be::FileSource(torrentFileName));
      bEncode.encode(be::FileDestination(
          Utility::createFileName(torrentFileName, ".json")));
      PLOG_INFO << "Created file "
                << Utility::createFileName(torrentFileName, ".json") << " from "
                << torrentFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "BencodeTorrent_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}