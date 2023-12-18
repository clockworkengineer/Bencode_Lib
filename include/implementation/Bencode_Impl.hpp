#pragma once

#include <string>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <memory>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Impl {

public:
  Bencode_Impl();
  Bencode_Impl(const Bencode_Impl &other) = delete;
  Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
  Bencode_Impl(Bencode_Impl &&other) = delete;
  Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
  ~Bencode_Impl();

  void setEncoder(IEncoder *encoder);
  void setDecoder(IDecoder *decoder);
  void decode(ISource &source);
  void encode(IDestination &destination) const;
  std::string version() const;
  [[nodiscard]] BNode &root() { return (bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (bNodeRoot); }
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](std::size_t index);
  const BNode &operator[](std::size_t index) const;

private:
  // Root of BNode tree
  BNode bNodeRoot;
  // Bencode encoder
  std::unique_ptr<IEncoder> bNodeEncoder{};
  // Bencode decoder
  std::unique_ptr<IDecoder> bNodeDecoder{};
};

} // namespace Bencode_Lib
