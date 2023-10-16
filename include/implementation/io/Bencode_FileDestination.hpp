#pragma once

#include <fstream>
#include <string>

#include "IDestination.hpp"

namespace Bencode_Lib {

class FileDestination : public IDestination {
public:
  // ======================
  explicit FileDestination(const std::string &destinationFileName) {
    m_destination.open(destinationFileName.c_str(), std::ios_base::binary);
    if (!m_destination.is_open()) {
      throw Error("Bencode file output stream failed to open or could not be "
                  "created.");
    }
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  virtual ~FileDestination() = default;

  void add(const std::string &bytes) override {
    m_destination.write(bytes.c_str(), bytes.length());
    m_destination.flush();
  }
  void add(const char ch) override {
    m_destination.put(ch);
    m_destination.flush();
  }

private:
  std::ofstream m_destination;
};
} // namespace Bencode_Lib