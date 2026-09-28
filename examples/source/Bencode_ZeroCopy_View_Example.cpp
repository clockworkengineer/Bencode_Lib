//
// File: Bencode_ZeroCopy_View_Example.cpp
//
// Description: Example illustrating zero-copy Bencode parsing and structured navigation using BencodeView and NodeView.
//

#include "Bencode.hpp"
#include "Bencode_View.hpp"

#include <iomanip>
#include <iostream>
#include <string_view>

int main() {
  std::cout << "========================================\n";
  std::cout << " Bencode_Lib Zero-Copy BencodeView Demo\n";
  std::cout << "========================================\n\n";

  // Sample torrent-like Bencode payload
  constexpr std::string_view sampleTorrent =
      "d"
        "8:announce27:http://tracker.example.com/"
        "4:infod"
          "6:lengthi1048576e"
          "4:name11:example.iso"
          "12:piece lengthi32768e"
          "6:pieces20:\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14"
        "e"
        "7:tags_inli1ei2ei3ee"
      "e";

  std::cout << "Raw Bencode input length: " << sampleTorrent.size() << " bytes\n\n";

  try {
    // Zero-copy parse: returns BencodeView without copying any strings or keys
    Bencode_Lib::BencodeView view = Bencode_Lib::BencodeView::parse(sampleTorrent);

    std::cout << "[1] Root Properties:\n";
    std::cout << "  - Root is dictionary: " << std::boolalpha << view.root().is_dict() << "\n";
    std::cout << "  - Root entry count:   " << view.root().size() << "\n\n";

    // Direct O(log K) dictionary indexing
    std::cout << "[2] Metadata Extraction:\n";
    std::cout << "  - Tracker URL: " << view["announce"].as_string().value() << "\n";

    auto info = view["info"];
    std::cout << "  - File name:   " << info["name"].as_string().value() << "\n";
    std::cout << "  - File length: " << info["length"].as_int().value() << " bytes\n";
    std::cout << "  - Piece length:" << info["piece length"].as_int().value() << " bytes\n";

    // Binary byte inspection without hex decoding overhead
    auto pieces = info["pieces"].as_binary().value();
    std::cout << "  - Pieces hash bytes: " << pieces.size() << " bytes (first 4 bytes: 0x"
              << std::hex << std::setfill('0')
              << std::setw(2) << static_cast<int>(pieces[0])
              << std::setw(2) << static_cast<int>(pieces[1])
              << std::setw(2) << static_cast<int>(pieces[2])
              << std::setw(2) << static_cast<int>(pieces[3])
              << std::dec << ")\n\n";

    // Structured binding dictionary iteration
    std::cout << "[3] Iterating Root Dictionary Entries (Sorted Keys):\n";
    auto dictEntries = view.root().as_dict().value();
    for (const auto &[key, val] : dictEntries) {
      std::cout << "  - Key: '" << key << "' -> Type: ";
      switch (val.type()) {
      case Bencode_Lib::NodeView::Type::Integer:
        std::cout << "Integer (" << val.as_int().value() << ")\n";
        break;
      case Bencode_Lib::NodeView::Type::String:
        std::cout << "String (\"" << val.as_string().value() << "\")\n";
        break;
      case Bencode_Lib::NodeView::Type::List:
        std::cout << "List (" << val.size() << " items)\n";
        break;
      case Bencode_Lib::NodeView::Type::Dictionary:
        std::cout << "Dictionary (" << val.size() << " entries)\n";
        break;
      default:
        std::cout << "Unknown\n";
        break;
      }
    }

    std::cout << "\nZero-copy parse and navigation completed successfully!\n";
    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Error during BencodeView parsing: " << ex.what() << "\n";
    return 1;
  }
}
