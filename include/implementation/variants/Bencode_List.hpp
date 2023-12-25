#pragma once

#include <vector>

namespace Bencode_Lib {

struct List : Variant {
  // Constructors/Destructors
  List() : Variant(Variant::Type::list){};
  explicit List(std::vector<BNode> &entryList)
      : Variant(Variant::Type::list), bNodeList(std::move(entryList)) {}
  // Add array element
  void add(BNode &bNode) { bNodeList.emplace_back(std::move(bNode)); }
  void add(BNode &&bNode) { bNodeList.emplace_back(std::move(bNode)); }
  [[nodiscard]] int size() const {
    return (static_cast<int>(bNodeList.size()));
  }
  [[nodiscard]] std::vector<BNode> &value() { return (bNodeList); }
  [[nodiscard]] const std::vector<BNode> &value() const { return (bNodeList); }
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
  std::vector<BNode> bNodeList;
};
} // namespace Bencode_Lib