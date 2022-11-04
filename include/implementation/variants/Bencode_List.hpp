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
// ====
// List
// ====
struct List : Variant {
  using Entry = BNode;
  using EntryList = std::vector<Entry>;
  explicit List(EntryList &entryList)
      : Variant(BNode::Type::list), m_list(std::move(entryList)) {}
  [[nodiscard]] int size() const { return (static_cast<int>(m_list.size())); }
  [[nodiscard]] EntryList &list() { return (m_list); }
  [[nodiscard]] const EntryList &list() const { return (m_list); }
  BNode &operator[](int index) {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }
  const BNode &operator[](int index) const {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }

private:
  EntryList m_list;
};

} // namespace Bencode_Lib