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
  enum class Type { base = 0, dictionary, list, integer, string };
  explicit Variant(Type nodeType = Type::base) : m_nodeType(nodeType) {}
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  [[nodiscard]] Type getNodeType() const { return (m_nodeType); }

private:
  Variant::Type m_nodeType;
};

} // namespace Bencode_Lib