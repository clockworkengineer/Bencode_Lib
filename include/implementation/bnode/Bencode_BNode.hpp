#pragma once
//
// C++ STL
//
#include <string>
#include <memory>
#include <stdexcept>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// =====
// BNode
// =====
struct BNode {
  // BNode Error
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  // Constructors/Destructors
  BNode() = default;
  explicit BNode(std::unique_ptr<Variant> bNodeVariant)
      : m_bNodeVariant(std::move(bNodeVariant)) {}
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
  // Get BNode type
  [[nodiscard]] Variant::Type getNodeType() const {
    return (m_bNodeVariant->getNodeType());
  }
  // Get reference to BNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant() {
    return (m_bNodeVariant);
  }
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const {
    return (m_bNodeVariant);
  }

private:
  std::unique_ptr<Variant> m_bNodeVariant;
};
} // namespace Bencode_Lib