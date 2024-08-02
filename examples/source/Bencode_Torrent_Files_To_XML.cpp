//
// Program:  Bencode_Bencode_Torrent_Files_To_XML
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as XML using a custom encoder.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "XML_Encoder.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    const be::Bencode bEncode(be::makeEncoder<be::XML_Encoder>());
    // Initialise logging.
    init(plog::debug, "Bencode_Torrent_Files_To_XML.log");
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
    PLOG_ERROR << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Bencode_Torrent_Files_To_XML exited.";
  exit(EXIT_SUCCESS);
}