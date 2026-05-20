#pragma once


#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include "Bencode.hpp"
#include "Bencode_Core.hpp"

class Utility {
public:
  static std::vector<std::string> createTorrentFileList() {
    std::vector<std::string> fileList;
    for (auto &file : std::filesystem::directory_iterator(
             (std::filesystem::current_path() / "files"))) {
      if (file.path().extension() == ".torrent") {
        fileList.push_back(file.path().string());
      }
    }
    std::sort(fileList.begin(), fileList.end());
    return fileList;
  }
  static std::string createFileName(const std::string &torrentFileName,
                                    const std::string &newExtension) {
    std::string newFileName = torrentFileName;
    return (
        newFileName.erase(newFileName.find(".torrent"), newFileName.length()) +
        newExtension);
  }
};