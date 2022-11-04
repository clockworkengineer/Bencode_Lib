#pragma once
//
// C++ STL
//
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ========
// Variants
// ========
struct Variant {
  explicit Variant(BNode::Type nodeType = BNode::Type::base)
      : m_nodeType(nodeType) {}
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  [[nodiscard]] BNode::Type getNodeType() const { return (m_nodeType); }

private:
  BNode::Type m_nodeType;
};

} // namespace Bencode_Lib