#pragma once

#include <string>
#include <stdexcept>
#include <limits>
#include <array>
#include <sstream>

#include "ISource.hpp"
#include "IDestination.hpp"

#include "Bencode_Config.hpp"
#include "Bencode_Types.hpp"

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
  BNode &root() { return (m_bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (m_bNodeRoot); }

private:
  static int64_t extractInteger(ISource &source);
  static std::string extractString(ISource &source);
  static BNode decodeString(ISource &source);
  static BNode decodeInteger(ISource &source);
  static BNode decodeDictionary(ISource &source);
  static BNode decodeList(ISource &source);
  static BNode decodeBNodes(ISource &source);
  static void encodeBNodes(const BNode &bNode, IDestination &destination);

  // Root of BNode tree
  BNode m_bNodeRoot;
};
} // namespace Bencode_Lib
