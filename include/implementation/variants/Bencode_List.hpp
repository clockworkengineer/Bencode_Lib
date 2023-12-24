#pragma once

#include <vector>

namespace Bencode_Lib {

struct List : Variant {
  using Entry = BNode;
  using EntryList = std::vector<Entry>;
  List() : Variant(Variant::Type::list){};
  explicit List(EntryList &entryList)
      : Variant(Variant::Type::list), bNodeList(std::move(entryList)) {}
  // Add array element
  void add(BNode &bNode) { bNodeList.emplace_back(std::move(bNode)); }
  void add(BNode &&bNode) { bNodeList.emplace_back(std::move(bNode)); }
  [[nodiscard]] int size() const {
    return (static_cast<int>(bNodeList.size()));
  }
  [[nodiscard]] EntryList &value() { return (bNodeList); }
  [[nodiscard]] const EntryList &value() const { return (bNodeList); }
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