#pragma once
// =======
// C++ STL
// =======
#include <fstream>
#include <string>
#include <vector>
// =====================
// Destination interface
// =====================
#include "IDestination.hpp"
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ================
// CLASS DEFINITION
// ================
class FileDestination : public IDestination {
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
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
  // ==============
  // PUBLIC METHODS
  // ==============
  void add(const std::string &bytes) override {
    m_destination.write(bytes.c_str(), bytes.length());
    m_destination.flush();
  }
  void add(const char ch) override {
    m_destination.put(ch);
    m_destination.flush();
  }
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  // =================
  // PRIVATE VARIABLES
  // =================
  std::ofstream m_destination;
};
} // namespace Bencode_Lib