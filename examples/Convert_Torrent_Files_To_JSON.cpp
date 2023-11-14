//
// Program: Convert_Torrent_Files_To_JSON
//
// Description: Use Bencode_Lib to read in torrent file details.
//
// Dependencies: C++20, Bencode_Lib.
//

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Sources.hpp"

#include "IDestination.hpp"
#include "IEncoder.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace fs = std::filesystem;

class Encoder_JSON : public Bencode_Lib::IEncoder {

public:
  // Constructors/Destructors
  Encoder_JSON() = default;
  Encoder_JSON(const Encoder_JSON &other) = delete;
  Encoder_JSON &operator=(const Encoder_JSON &other) = delete;
  Encoder_JSON(Encoder_JSON &&other) = delete;
  Encoder_JSON &operator=(Encoder_JSON &&other) = delete;
  virtual ~Encoder_JSON() = default;

  void encode(const Bencode_Lib::BNode &bNode,
              Bencode_Lib::IDestination &destination) const {
    if (bNode.isDictionary()) {
      destination.add('d');
      for (const auto &bNodeNext :
           BRef<Bencode_Lib::Dictionary>(bNode).dictionary()) {
        destination.add(std::to_string(bNodeNext.first.length()) + ":" +
                        bNodeNext.first);
        encode(bNodeNext.second, destination);
      }
      destination.add('e');
    } else if (bNode.isList()) {
      destination.add('l');
      for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).list()) {
        encode(bNodeNext, destination);
      }
      destination.add('e');
    } else if (bNode.isInteger()) {
      destination.add('i');
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).integer()));
      destination.add('e');
    } else if (bNode.isString()) {
      destination.add(std::to_string(static_cast<int>(
                          BRef<Bencode_Lib::String>(bNode).string().length())) +
                      ":" + BRef<Bencode_Lib::String>(bNode).string());
    }
  }
};

/// <summary>
/// Return a vector of torrent files to analyze.
/// </summary>
/// <returns>Vector of torrent file names</returns>
std::vector<std::string> readTorrentFileList() {
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator((fs::current_path() / "files"))) {
    if (const auto fileName = file.path().string();
        fileName.ends_with(".torrent")) {
      fileList.push_back(fileName);
    }
  }
  return (fileList);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Convert_Torrent_Files_To_JSON.log");
    PLOG_INFO << "Convert_Torrent_Files_To_JSON started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    std::unique_ptr<Encoder_JSON> jsonEncoder = std::make_unique<Encoder_JSON>();
    // For each torrent file
    for (const auto &fileName : readTorrentFileList()) {
      Bencode_Lib::Bencode bEncode(jsonEncoder.release(), nullptr);
      bEncode.decode(Bencode_Lib::FileSource(fileName));
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Convert_Torrent_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}