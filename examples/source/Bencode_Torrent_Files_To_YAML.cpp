//
// Program:  Bencode_Bencode_Files_To_YAML
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as YAML using a custom encoder.
//
// Dependencies: C++20, PLOG,  Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "YAML_Encoder.hpp"

namespace be = Bencode_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    try {
        const be::Bencode bEncode(be::makeEncoder<be::YAML_Encoder>());
        // Initialise logging.
        init(plog::debug, "Bencode_Files_To_YAML.log");
        PLOG_INFO << "Bencode_Files_To_YAML started ...";
        PLOG_INFO << be::Bencode::version();
        for (const auto &fileName : Utility::createTorrentFileList()) {
            bEncode.decode(be::FileSource(fileName));
            bEncode.encode(be::FileDestination(
                Utility::createFileName(fileName, ".yaml")));
            PLOG_INFO << "Created file "
                      << Utility::createFileName(fileName, ".yaml") << " from "
                      << fileName;
        }
    } catch (const std::exception &ex) {
        PLOG_ERROR << "Error Processing Bencode File: [" << ex.what() << "]\n";
    }
    PLOG_INFO << "Bencode_Files_To_YAML exited.";
    exit(EXIT_SUCCESS);
}