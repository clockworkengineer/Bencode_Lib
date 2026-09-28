//
// File: Bencode_Modules_Example.cpp
//
// Description: Demonstrates consuming Bencode_Lib using modern C++23 Named Modules (`import Bencode_Lib;`).
//

import Bencode_Lib;

#include <iostream>
#include <string>
#include <string_view>

struct TorrentEntry {
  std::string announce;
  int64_t pieceLength{0};
};

void to_bencode(Bencode_Lib::Node &node, const TorrentEntry &t) {
  node = Bencode_Lib::Node::make<Bencode_Lib::Dictionary>();
  Bencode_Lib::to_bencode_field(node, "announce", t.announce);
  Bencode_Lib::to_bencode_field(node, "piece length", t.pieceLength);
}

void from_bencode(const Bencode_Lib::Node &node, TorrentEntry &t) {
  Bencode_Lib::from_bencode_field(node, "announce", t.announce);
  Bencode_Lib::from_bencode_field(node, "piece length", t.pieceLength);
}

int main() {
  std::cout << "===========================================\n";
  std::cout << " Bencode_Lib C++23 Modules Example\n";
  std::cout << "===========================================\n\n";

  // 1. DOM usage via imported module
  Bencode_Lib::Bencode bencode;
  bencode.parse("d4:ranki1e4:user3:Bobe");
  std::cout << "[1] Parsed via import Bencode_Lib:\n";
  std::cout << "  - User: " << bencode["user"].as_string().value_or("") << "\n";
  std::cout << "  - Rank: " << bencode["rank"].as_int().value_or(0) << "\n\n";

  // 2. Zero-copy AST view via imported module
  std::string_view raw = "d8:announce27:http://tracker.example.com/e";
  auto view = Bencode_Lib::BencodeView::parse(raw);
  std::cout << "[2] Zero-copy view lookup:\n";
  std::cout << "  - Announce: " << view["announce"].as_string().value_or("") << "\n\n";

  // 3. Struct mapping via imported module
  TorrentEntry entry{"http://tracker.archlinux.org:6969/announce", 524288};
  Bencode_Lib::Bencode objBencode = Bencode_Lib::Bencode::from_object(entry);
  std::cout << "[3] Serialized struct:\n";
  std::cout << "  - Encoded: " << objBencode.encode() << "\n\n";

  TorrentEntry decoded = objBencode.get<TorrentEntry>();
  std::cout << "[4] Deserialized struct:\n";
  std::cout << "  - Tracker: " << decoded.announce << "\n";
  std::cout << "  - Piece length: " << decoded.pieceLength << " bytes\n\n";

  std::cout << "C++23 Modules demonstration completed successfully!\n";
  return 0;
}
