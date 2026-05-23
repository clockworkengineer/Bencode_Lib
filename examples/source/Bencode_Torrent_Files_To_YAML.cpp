// File: Bencode_Torrent_Files_To_YAML.cpp
//
// Program:  Bencode_Files_To_YAML
//
// Description: Use Bencode_Lib to read in torrent file then write
// it out as YAML using a custom stringify.
//
// Dependencies: C++20, Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "YAML_Stringify.hpp"

namespace be = Bencode_Lib;

/// <summary>
/// Example entry point that converts torrent metadata to YAML.
/// </summary>
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    try {
        const be::Bencode bStringify(be::makeStringify<be::YAML_Stringify>());
        // Initialise logging.
        std::cout << "Bencode_Files_To_YAML started ...";
        std::cout << be::Bencode::version();
        for (const auto &fileName : Utility::createTorrentFileList()) {
            bStringify.parse(be::FileSource(fileName));
            bStringify.stringify(be::FileDestination(
                Utility::createFileName(fileName, ".yaml")));
            std::cout << "Created file "
                      << Utility::createFileName(fileName, ".yaml") << " from "
                      << fileName;
        }
    } catch (const std::exception &ex) {
        std::cerr << "Error Processing Bencode File: [" << ex.what() << "]\n";
    }
    std::cout << "Bencode_Files_To_YAML exited.";
    exit(EXIT_SUCCESS);
}