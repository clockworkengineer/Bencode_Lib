// Program: Bencode_Large_File_Example
// Description: Demonstrates reading and writing large Bencode files using Bencode_Lib.
// Dependencies: C++20, Bencode_Lib.

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include <iostream>
#include <fstream>

namespace be = Bencode_Lib;

int main() {
    // Read Bencode data from a file
    std::string fileData = be::Bencode::fromFile("large_example.torrent");
    be::Bencode doc(fileData);
    // Print root node type (as integer value)
    // 0: integer, 1: string, 2: list, 3: dictionary (see Variant::Type enum in docs)
    std::cout << "Root node type: " << static_cast<int>(doc.root().getVariant().getNodeType()) << std::endl;

    // Modify the document (add a new key)
    doc["new_key"] = std::string("new_value");

    // Write back to a file
    // Encode the modified document to a string
    be::BufferDestination dest;
    doc.stringify(dest);
    std::string encoded = dest.toString();
    be::Bencode::toFile("large_example_modified.torrent", encoded);
    std::cout << "Modified file written to large_example_modified.torrent" << std::endl;
    return 0;
}
