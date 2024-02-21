//
// Program:  Bencode_Bencode_Torrent_Files_To_XML
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as XML using a custom encoder.
//
// Dependencies: C++20, Bencode_Lib.
//

#include <stdexcept>

#include "Utility.hpp"
#include "XML_Encoder.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    be::Bencode bEncode(std::make_unique<be::XML_Encoder>(be::XML_Translator()).release());
    // Initialise logging.
    plog::init(plog::debug, "Bencode_Torrent_Files_To_XML.log");
    PLOG_INFO << "Bencode_Torrent_Files_To_XML started ...";
    PLOG_INFO << be::Bencode().version();
    for (const auto &torrentFileName : Utility::createTorrentFileList()) {
      bEncode.decode(be::FileSource(torrentFileName));
      bEncode.encode(be::FileDestination(
          Utility::createFileName(torrentFileName, ".xml")));
      PLOG_INFO << "Created file "
                << Utility::createFileName(torrentFileName, ".xml") << " from "
                << torrentFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Bencode_Torrent_Files_To_XML exited.";
  exit(EXIT_SUCCESS);
}