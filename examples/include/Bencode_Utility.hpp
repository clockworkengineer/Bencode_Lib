// File: Bencode_Utility.hpp
//
// Description: Utility helpers for example applications, including file list
// creation and file name normalization.
//
#pragma once

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <string>
#include "Bencode.hpp"
#include "Bencode_Core.hpp"

class Utility {
public:
  /// <summary>
  /// Create a sorted list of all torrent file paths in the example files directory.
  /// </summary>
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
  /// <summary>
  /// Convert a torrent file name into a file name with a new extension.
  /// </summary>
  static std::string createFileName(const std::string &torrentFileName,
                                    const std::string &newExtension) {
    std::string newFileName = torrentFileName;
    return (
        newFileName.erase(newFileName.find(".torrent"), newFileName.length()) +
        newExtension);
  }
};