#pragma once

#include <string>
#include <memory>
#include <stdexcept>

namespace Bencode_Lib {

struct BNode {
  // BNode Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  // Constructors/Destructors
  BNode() = default;
  template <typename T> explicit BNode(T value);
  BNode(const Bencode::ListInitializer &list);
  BNode(const Bencode::DictionaryInitializer &DictionaryInitializer);
  BNode(const BNode &other) = delete;
  BNode &operator=(const BNode &other) = delete;
  BNode(BNode &&other) = default;
  BNode &operator=(BNode &&other) = default;
  ~BNode() = default;
  // Assignment operators
  template <typename T> BNode &operator=(T value) {
    return (*this = BNode(value));
  }
  // Indexing operators
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  // Determine BNode type
  [[nodiscard]] bool isEmpty() const { return (bNodeVariant == nullptr); }
  [[nodiscard]] bool isString() const {
    return (bNodeVariant->getNodeType() == Variant::Type::string);
  }
  [[nodiscard]] bool isInteger() const {
    return (bNodeVariant->getNodeType() == Variant::Type::integer);
  }
  [[nodiscard]] bool isList() const {
    return (bNodeVariant->getNodeType() == Variant::Type::list);
  }
  [[nodiscard]] bool isDictionary() const {
    return (bNodeVariant->getNodeType() == Variant::Type::dictionary);
  }
  // Get reference to BNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant() {
    return (bNodeVariant);
  }
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const {
    return (bNodeVariant);
  }
  // Make BNode
  template <typename T, typename... Args> static auto make(Args &&...args) {
    return (BNode{std::make_unique<T>(std::forward<Args>(args)...)});
  }

private:
  static BNode internalTypeToBNode(const Bencode::InternalType &type);
  std::unique_ptr<Variant> bNodeVariant;
};

} // namespace Bencode_Lib