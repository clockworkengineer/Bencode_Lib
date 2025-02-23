//
// Program:  Bencode_Files_To_JSON
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "JSON_Encoder.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    const be::Bencode bEncode(be::makeEncoder<be::JSON_Encoder>());
    // Initialise logging.
    init(plog::debug, "Bencode_Files_To_JSON.log");
    PLOG_INFO << "Bencode_Files_To_JSON started ...";
    PLOG_INFO << be::Bencode::version();
    for (const auto &fileName : Utility::createTorrentFileList()) {
      bEncode.decode(be::FileSource(fileName));
      bEncode.encode(be::FileDestination(
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