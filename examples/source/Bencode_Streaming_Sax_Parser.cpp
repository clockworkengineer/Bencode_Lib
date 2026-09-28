// File: Bencode_Streaming_Sax_Parser.cpp
//
// Description: Example demonstrating event-driven streaming SAX parsing
// with zero-copy string views and early termination.
//

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "interface/ISaxHandler.hpp"

#include <iostream>
#include <string>
#include <string_view>

using namespace Bencode_Lib;

// A custom SAX handler that filters for specific metadata keys
class TorrentMetadataHandler : public ISaxHandler {
public:
  std::string announceUrl;
  int64_t pieceLength = 0;
  std::size_t pieceHashCount = 0;

  bool on_integer(int64_t value) override {
    if (currentKey == "piece length") {
      pieceLength = value;
      std::cout << "  [Event] piece length = " << pieceLength << " bytes\n";
    }
    return true;
  }

  bool on_string(std::string_view value) override {
    if (currentKey == "announce") {
      announceUrl = std::string(value);
      std::cout << "  [Event] announce URL = " << announceUrl << "\n";
    } else if (currentKey == "pieces") {
      // BitTorrent SHA-1 hashes are 20 bytes each
      pieceHashCount = value.size() / 20;
      std::cout << "  [Event] pieces total bytes = " << value.size()
                << " (" << pieceHashCount << " SHA-1 hashes)\n";
    }
    return true;
  }

  bool on_list_begin() override { return true; }
  bool on_list_end() override { return true; }
  bool on_dictionary_begin() override { return true; }

  bool on_dictionary_key(std::string_view key) override {
    currentKey = key;
    return true;
  }

  bool on_dictionary_end() override { return true; }

private:
  std::string_view currentKey;
};

// A fast lookup handler demonstrating early termination
class FastTrackerFinder : public ISaxHandler {
public:
  std::string tracker;

  bool on_integer(int64_t) override { return true; }

  bool on_string(std::string_view value) override {
    if (currentKey == "announce") {
      tracker = std::string(value);
      std::cout << "  [Early Exit] Found announce tracker: " << tracker
                << ". Aborting remaining parse!\n";
      return false; // Stop parsing immediately!
    }
    return true;
  }

  bool on_list_begin() override { return true; }
  bool on_list_end() override { return true; }
  bool on_dictionary_begin() override { return true; }

  bool on_dictionary_key(std::string_view key) override {
    currentKey = key;
    return true;
  }

  bool on_dictionary_end() override { return true; }

private:
  std::string_view currentKey;
};

int main() {
  std::cout << "========================================\n";
  std::cout << "Bencode_Lib Streaming SAX Parser Example\n";
  std::cout << "========================================\n\n";

  // Sample torrent payload
  const std::string mockPieces(20 * 3, '\xAB'); // 3 dummy SHA-1 hashes (60 bytes)
  const std::string torrentData =
      "d"
      "8:announce27:http://tracker.example.com/"
      "4:infod"
      "12:piece lengthi262144e"
      "6:pieces60:" + mockPieces +
      "e"
      "e";

  std::cout << "1. Full SAX Streaming Parse:\n";
  TorrentMetadataHandler metadataHandler;
  bool completed = Bencode::parseSax(torrentData, metadataHandler);
  if (completed) {
    std::cout << "Streaming parse completed successfully without materializing DOM!\n";
  }

  std::cout << "\n2. Early Termination SAX Parse:\n";
  FastTrackerFinder finder;
  bool completedFinder = Bencode::parseSax(torrentData, finder);
  std::cout << "Parser returned completed = " << std::boolalpha << completedFinder
            << " (false indicates early exit requested by handler).\n";

  return 0;
}
