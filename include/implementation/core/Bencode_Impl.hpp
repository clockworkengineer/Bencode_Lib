#pragma once

#include <string>
#include <stdexcept>
#include <limits>
#include <array>
#include <sstream>
#include <memory>

#include "ISource.hpp"
#include "IDestination.hpp"

#include "Bencode_Config.hpp"
#include "Bencode_Types.hpp"

#include "Decoder_Default.hpp"
#include "Encoder_Default.hpp"

namespace Bencode_Lib {

class Bencode_Impl {
public:
  Bencode_Impl() = default;
  Bencode_Impl(const Bencode_Impl &other) = delete;
  Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
  Bencode_Impl(Bencode_Impl &&other) = delete;
  Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
  ~Bencode_Impl() = default;

  void setEncoder(IEncoder *encoder);
  void setDecoder(IDecoder *decoder);
  void decode(ISource &source);
  void encode(IDestination &destination) const;
  std::string version() const;
  BNode &root() { return (bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (bNodeRoot); }

private:
  // Root of BNode tree
  BNode bNodeRoot;
  // Bencode encoder default
  std::unique_ptr<IEncoder> bNodeEncoder{};
  // Bencode decoder default
  std::unique_ptr<IDecoder> bNodeDecoder{};
};
} // namespace Bencode_Lib
