#pragma once

#include <string>
#include <memory>
#include <variant>
#include <stdexcept>
#include <type_traits>
#include <array>
#include <limits>
#include <functional>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>
#include <cstring>

#include "Bencode_Config.hpp"
#include "Bencode_Interfaces.hpp"

namespace Bencode_Lib {

// ============================
// Bencode forward declarations
// ============================
class Bencode_Impl;
struct BNode;

class Bencode {

public:
  // Possible Bencode BNode initializer types
  using InitializerListTypes =
      std::variant<int, long, long long, float, double, long double, bool,
                   std::string, std::nullptr_t, BNode>;
  // List initializer list
  using ListInitializer = std::initializer_list<InitializerListTypes>;
  // Dictionary initializer list
  using DictionaryInitializer =
      std::initializer_list<std::pair<std::string, InitializerListTypes>>;
  // Constructors/Destructors
  explicit Bencode([[maybe_unused]] IEncoder *encoder = nullptr,
                   [[maybe_unused]] IDecoder *decoder = nullptr);
  // Pass in default JSON to parse
  explicit Bencode(const std::string &bencodeString);
  // Construct an array
  Bencode(const ListInitializer &list);
  // Construct object
  Bencode(const DictionaryInitializer &dictionary);
  Bencode(const Bencode &other) = delete;
  Bencode &operator=(const Bencode &other) = delete;
  Bencode(Bencode &&other) = delete;
  Bencode &operator=(Bencode &&other) = delete;
  ~Bencode();
  // Decode Bencode into BNode tree
  void decode(ISource &source) const;
  void decode(ISource &&source) const;
  // Encode Bencode from BNode tree
  void encode(IDestination &destination) const;
  void encode(IDestination &&destination) const;
  // Return Bencode_Lib version
  [[nodiscard]] static std::string version();
  // Return BNode tree root
  [[nodiscard]] BNode &root();
  [[nodiscard]] const BNode &root() const;
  // Traverse Bencode tree
  [[maybe_unused]] void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Search for Bencode dictionary entry with a given key
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  // Get Bencode list entry at index
  BNode &operator[](std::size_t index);
  const BNode &operator[](std::size_t index) const;

private:
  const std::unique_ptr<Bencode_Impl> implementation;
};

} // namespace Bencode_Lib
