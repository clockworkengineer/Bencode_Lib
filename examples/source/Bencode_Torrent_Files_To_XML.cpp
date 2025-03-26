//
// Program:  Bencode_Bencode_Files_To_XML
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as XML using a custom stringify.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "XML_Stringify.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    const be::Bencode bStringify(be::makeStringify<be::XML_Stringify>());
    // Initialise logging.
    init(plog::debug, "Bencode_Files_To_XML.log");
    PLOG_INFO << "Bencode_Files_To_XML started ...";
    PLOG_INFO << be::Bencode::version();
    for (const auto &fileName : Utility::createTorrentFileList()) {
      bStringify.parse(be::FileSource(fileName));
      bStringify.stringify(be::FileDestination(
          Utility::createFileName(fileName, ".xml")));
      PLOG_INFO << "Created file "
                << Utility::createFileName(fileName, ".xml") << " from "
                << fileName;
    }
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error Processing Bencode File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Bencode_Files_To_XML exited.";
  exit(EXIT_SUCCESS);
}