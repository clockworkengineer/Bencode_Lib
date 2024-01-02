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

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    Bencode_Lib::Bencode bEncode(std::make_unique<XML_Encoder>().release());
    // Initialise logging.
    plog::init(plog::debug, "Bencode_Torrent_Files_To_XML.log");
    PLOG_INFO << "Bencode_Torrent_Files_To_XML started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    for (const auto &torrentFileName : Utility::createTorrentFileList()) {
      bEncode.decode(Bencode_Lib::FileSource(torrentFileName));
      bEncode.encode(Bencode_Lib::FileDestination(
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