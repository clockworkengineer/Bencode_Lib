#pragma once

#include <filesystem>
#include <vector>
#include <string>

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
  static std::string createFileName(const std::string &torrentFileName,
                                    const std::string newExtension) {
    std::string newFileName = torrentFileName;
    return (
        newFileName.erase(newFileName.find(".torrent"), newFileName.length()) +
        newExtension);
  }
  static std::string bufferToString(const std::vector<std::byte> &buffer) {
    std::string destination;
    for (auto ch : buffer) {
      destination.push_back(static_cast<char>(ch));
    }
    return (destination);
  }
};