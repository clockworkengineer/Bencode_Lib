#pragma once
//
// C++ STL
//
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// =======
// Integer
// =======
struct Integer : Variant {
  explicit Integer(int64_t integer)
      : Variant(BNode::Type::integer), m_value(integer) {}
  [[nodiscard]] int64_t integer() const { return (m_value); }

private:
  int64_t m_value = 0;
};

} // namespace Bencode_Lib