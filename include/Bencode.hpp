#pragma once

#include <string>
#include <stdexcept>
#include <memory>

namespace Bencode_Lib {

#include "Bencode_Forward.hpp"

class Bencode {
public:
  Bencode();
  Bencode(const Bencode &other) = delete;
  Bencode &operator=(const Bencode &other) = delete;
  Bencode(Bencode &&other) = delete;
  Bencode &operator=(Bencode &&other) = delete;
  ~Bencode();

  void decode(ISource &source) const;
  void decode(ISource &&source) const;
  void encode(IDestination &destination) const;
  void encode(IDestination &&destination) const;
  [[nodiscard]] std::string version() const;
  [[nodiscard]] BNode &root();
  [[nodiscard]] const BNode &root() const;

private:
  const std::unique_ptr<Bencode_Impl> m_implementation;
};
} // namespace Bencode_Lib
