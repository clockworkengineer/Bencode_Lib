#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace Bencode_Lib {
// =====
// BNode
// =====
struct Variant;
struct BNode {
  enum class Type { base = 0, dictionary, list, integer, string };
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  BNode() = default;
  explicit BNode(std::unique_ptr<Variant> bNodeVariant)
      : m_bNodeVariant(std::move(bNodeVariant)) {}
  BNode(const BNode &other) = delete;
  BNode &operator=(const BNode &other) = delete;
  BNode(BNode &&other) = default;
  BNode &operator=(BNode &&other) = default;
  ~BNode() = default;
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  [[nodiscard]] Type getNodeType() const;
  // Get reference to BNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant();
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const;

private:
  std::unique_ptr<Variant> m_bNodeVariant;
};
} // namespace Bencode_Lib