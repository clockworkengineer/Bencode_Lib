//
// Program:  Bencode_Files_To_JSON
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as JSON using a custom stringify.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "JSON_Stringify.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    const be::Bencode bStringify(be::makeStringify<be::JSON_Stringify>());
    // Initialise logging.
    init(plog::debug, "Bencode_Files_To_JSON.log");
    PLOG_INFO << "Bencode_Files_To_JSON started ...";
    PLOG_INFO << be::Bencode::version();
    for (const auto &fileName : Utility::createTorrentFileList()) {
      bStringify.parse(be::FileSource(fileName));
      bStringify.stringify(be::FileDestination(
          Utility::createFileName(fileName, ".json")));
      PLOG_INFO << "Created file "
                << Utility::createFileName(fileName, ".json") << " from "
                << fileName;
    }
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error Processing Bencode File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Bencode_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}