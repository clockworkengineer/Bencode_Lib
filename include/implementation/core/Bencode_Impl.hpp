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

  void decode(ISource &source);
  void encode(IDestination &destination) const;
  std::string version() const;
  BNode &root() { return (bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (bNodeRoot); }

private:
  static int64_t extractInteger(ISource &source);
  static std::string extractString(ISource &source);
  static BNode decodeString(ISource &source);
  static BNode decodeInteger(ISource &source);
  static BNode decodeDictionary(ISource &source);
  static BNode decodeList(ISource &source);
  static BNode decodeBNode(ISource &source);
  static void encodeBNode(const BNode &bNode, IDestination &destination);

  // Root of BNode tree
  BNode bNodeRoot;
  // Bencode encoder default to Bencode
  std::unique_ptr<IEncoder> encoder = std::make_unique<Encoder_Default>();
};
} // namespace Bencode_Lib
