#pragma once

#include <string>
#include <stdexcept>

namespace Bencode_Lib {

class IDestination {
public:
  IDestination() = default;
  IDestination(const IDestination &other) = delete;
  IDestination &operator=(const IDestination &other) = delete;
  IDestination(IDestination &&other) = delete;
  IDestination &operator=(IDestination &&other) = delete;
  virtual ~IDestination() = default;
  // ========================
  // Add bytes to destination
  // ========================
  virtual void add(const std::string &bytes) = 0;
  // ============================
  // Add character to destination
  // ============================
  virtual void add(char ch) = 0;
};
} // namespace Bencode_Lib