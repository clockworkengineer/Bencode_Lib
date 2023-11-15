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
#include "Bencode_Destinations.hpp"

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
      destination.add('{');
      int commas = BRef<Bencode_Lib::Dictionary>(bNode).dictionary().size();
      for (const auto &bNodeNext :
           BRef<Bencode_Lib::Dictionary>(bNode).dictionary()) {
        destination.add("\"" + bNodeNext.first + "\"" + " : ");
        encode(bNodeNext.second, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add('}');
    } else if (bNode.isList()) {
      int commas = BRef<Bencode_Lib::List>(bNode).list().size();
      destination.add('[');
      for (const auto &bNodeNext : BRef<Bencode_Lib::List>(bNode).list()) {
        encode(bNodeNext, destination);
        if (--commas > 0)
          destination.add(",");
      }
      destination.add(']');
    } else if (bNode.isInteger()) {
      destination.add(
          std::to_string(BRef<Bencode_Lib::Integer>(bNode).integer()));
    } else if (bNode.isString()) {
      destination.add("\"" + BRef<Bencode_Lib::String>(bNode).string() + "\"");
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

std::string addJSONExtenstion(const std::string &fileName) {
  std::string jsonFilename = fileName;
  std::string::size_type i = jsonFilename.find(".torrent");
  if (i != std::string::npos) {
    jsonFilename.erase(i, jsonFilename.length());
    return (jsonFilename + ".json");
  }
  return ("");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Convert_Torrent_Files_To_JSON.log");
    PLOG_INFO << "Convert_Torrent_Files_To_JSON started ...";
    PLOG_INFO << Bencode_Lib::Bencode().version();
    for (const auto &fileName : readTorrentFileList()) {
      Bencode_Lib::Bencode bEncode(std::make_unique<Encoder_JSON>().release(),
                                   nullptr);
      bEncode.decode(Bencode_Lib::FileSource(fileName));
      std::string jsonFilename = addJSONExtenstion(fileName);
      if (jsonFilename != "") {
        std::cout << jsonFilename << std::endl;
        bEncode.encode(Bencode_Lib::FileDestination(jsonFilename));
      }
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "Convert_Torrent_Files_To_JSON exited.";
  exit(EXIT_SUCCESS);
}