#pragma once

#include <vector>

namespace Bencode_Lib {

struct List : Variant {
  using Entry = BNode;
  using EntryList = std::vector<Entry>;
  explicit List(EntryList &entryList)
      : Variant(Variant::Type::list), bNodeList(std::move(entryList)) {}
  [[nodiscard]] int size() const { return (static_cast<int>(bNodeList.size())); }
  [[nodiscard]] EntryList &list() { return (bNodeList); }
  [[nodiscard]] const EntryList &list() const { return (bNodeList); }
  BNode &operator[](int index) {
    if ((index >= 0) && (index < (static_cast<int>(bNodeList.size())))) {
      return (bNodeList[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }
  const BNode &operator[](int index) const {
    if ((index >= 0) && (index < (static_cast<int>(bNodeList.size())))) {
      return (bNodeList[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }

private:
  EntryList bNodeList;
};
} // namespace Bencode_Lib