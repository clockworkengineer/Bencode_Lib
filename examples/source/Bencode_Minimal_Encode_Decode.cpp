
// Program: Bencode_Minimal_Encode_Decode
// Description: Minimal example of encoding and decoding using Bencode_Lib.
// Dependencies: C++20, Bencode_Lib.

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

int main() {
    // Encode a dictionary
    be::Bencode bencodeDict;
    bencodeDict["name"] = std::string("John");
    bencodeDict["age"] = std::string("30");
    std::string encoded;
    {
        be::BufferDestination dest;
        bencodeDict.stringify(dest);
        encoded = dest.toString();
    }
    PLOG_INFO << "Bencoded string: " << encoded << std::endl;

    // Decode the string
    be::Bencode decoded;
    {
        be::BufferSource src(encoded);
        decoded.parse(src);
    }
    PLOG_INFO << "Decoded name: " << be::NRef<be::String>(decoded["name"]).value() << std::endl;
    PLOG_INFO << "Decoded age: " << be::NRef<be::String>(decoded["age"]).value() << std::endl;
    return 0;
}
