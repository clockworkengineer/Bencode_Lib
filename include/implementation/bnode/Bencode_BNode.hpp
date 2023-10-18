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
  explicit BNode(std::unique_ptr<Variant> bNodeVariant)
      : bNodeVariant(std::move(bNodeVariant)) {}
  BNode(const BNode &other) = delete;
  BNode &operator=(const BNode &other) = delete;
  BNode(BNode &&other) = default;
  BNode &operator=(BNode &&other) = default;
  ~BNode() = default;
  // Indexing operators
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  // Determine BNode type
  [[nodiscard]]bool is_string() const {
    return (bNodeVariant->getNodeType() ==  Variant::Type::string);
  }
  [[nodiscard]]bool is_integer() const {
    return (bNodeVariant->getNodeType() ==  Variant::Type::integer);
  }
  [[nodiscard]]bool is_list() const {
    return (bNodeVariant->getNodeType() ==  Variant::Type::list);
  }
  [[nodiscard]]bool is_dictionary() const {
    return (bNodeVariant->getNodeType() ==  Variant::Type::dictionary);
  }
  // Get reference to BNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant() {
    return (bNodeVariant);
  }
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const {
    return (bNodeVariant);
  }
  // Make BNode
  template <typename T, typename U> static BNode make(U &&value) {
    return (BNode{std::make_unique<T>(T{value})});
  }
  template <typename T> static BNode make() {
    return (BNode{std::make_unique<T>()});
  }

private:
  std::unique_ptr<Variant> bNodeVariant;
};
} // namespace Bencode_Lib