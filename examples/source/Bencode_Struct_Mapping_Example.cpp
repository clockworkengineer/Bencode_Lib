//
// File: Bencode_Struct_Mapping_Example.cpp
//
// Description: Example illustrating C++23 struct object mapping to/from Bencode in under 10 lines of code.
//

#include "Bencode.hpp"
#include <iostream>
#include <vector>

// Define BitTorrent domain structs
struct TorrentInfo {
  int64_t length = 0;
  std::string name;
  int64_t piece_length = 0;
  std::vector<std::byte> pieces;
};
BENCODE_STRUCT(TorrentInfo, (length, "length"), (name, "name"), (piece_length, "piece length"), (pieces, "pieces"))

struct Metainfo {
  std::string announce;
  TorrentInfo info;
  std::optional<std::string> comment;
};
BENCODE_DEFINE_TYPE_NON_INTRUSIVE(Metainfo, announce, info, comment)

int main() {
  std::cout << "===========================================\n";
  std::cout << " Bencode_Lib C++23 Struct Mapping Example\n";
  std::cout << "===========================================\n\n";

  // 1. Populate C++ domain object
  Metainfo originalTorrent{
      "http://tracker.archlinux.org:6969/announce",
      {3879731200LL, "archlinux-x86_64.iso", 524288, {std::byte{0xDE}, std::byte{0xAD}, std::byte{0xBE}, std::byte{0xEF}}},
      "Arch Linux 2026 Release"};

  // 2. Encode to canonical Bencode string in a single line
  std::string bencoded = Bencode_Lib::Bencode::from_object(originalTorrent).encode();
  std::cout << "[1] Encoded Bencode Payload (" << bencoded.size() << " bytes):\n";
  std::cout << bencoded.substr(0, 100) << "... [truncated]\n\n";

  // 3. Parse and deserialize directly back into C++ struct in a single line
  Metainfo decoded = Bencode_Lib::Bencode::parse_object<Metainfo>(bencoded);

  std::cout << "[2] Unpacked C++ Metainfo Struct:\n";
  std::cout << "  - Tracker URL: " << decoded.announce << "\n";
  std::cout << "  - File name:   " << decoded.info.name << "\n";
  std::cout << "  - File length: " << decoded.info.length << " bytes (" << (decoded.info.length / (1024 * 1024)) << " MB)\n";
  std::cout << "  - Piece size:  " << decoded.info.piece_length << " bytes\n";
  std::cout << "  - Pieces byte count: " << decoded.info.pieces.size() << " bytes\n";
  std::cout << "  - Comment:     " << decoded.comment.value_or("none") << "\n\n";

  std::cout << "Struct serialization and deserialization completed successfully!\n";
  return 0;
}
