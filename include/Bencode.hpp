#pragma once

#include <string>
#include <stdexcept>
#include <memory>
#include <variant>

#include "Bencode_Config.hpp"

namespace Bencode_Lib {

// ===========================================================
// Bencode forward declarations for interfaces/classes/structs
// ===========================================================
class ISource;
class IDestination;
class IEncoder;
class IDecoder;
class Bencode_Impl;
struct BNode;

class Bencode {

public:
  // Bencode error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Bencode Error: " + message) {}
  };
  // Possible Bencode Node internal value types
  using InternalType =
      std::variant<int, long, long long, float, double, long double, bool,
                   std::string, std::nullptr_t, BNode>;
  // List initializer list
  using ListInitializer = std::initializer_list<InternalType>;
  // Dictionary initializer list
  using DictionaryInitializer =
      std::initializer_list<std::pair<std::string, InternalType>>;
  // Constructors/Destructors
  Bencode(IEncoder *encoder = nullptr, IDecoder *decoder = nullptr);
  // Pass in default JSON to parse
  explicit Bencode(const std::string &bencodeString);
  // Construct array
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
  [[nodiscard]] std::string version() const;

  // Return BNode tree root
  [[nodiscard]] BNode &root();
  [[nodiscard]] const BNode &root() const;

  // Search for Bencode dictinary entry with a given key
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;

  // Get Bencode list entry at index
  BNode &operator[](std::size_t index);
  const BNode &operator[](std::size_t index) const;

private:
  const std::unique_ptr<Bencode_Impl> implementation;
};

} // namespace Bencode_Lib
