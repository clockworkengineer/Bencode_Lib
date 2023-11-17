#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>

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
    return (fileList);
  }
};