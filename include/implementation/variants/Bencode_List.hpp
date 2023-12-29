#pragma once

#include <vector>

namespace Bencode_Lib {

struct List : Variant {
  // Constructors/Destructors
  List() : Variant(Variant::Type::list){};
  List(const List &other) = default;
  List &operator=(const List &other) = default;
  List(List &&other) = default;
  List &operator=(List &&other) = default;
  ~List() = default;
  // Add array element
  void add(BNode &bNode) { bNodeList.emplace_back(std::move(bNode)); }
  void add(BNode &&bNode) { bNodeList.emplace_back(std::move(bNode)); }
  // Get BNode size
  [[nodiscard]] int size() const {
    return (static_cast<int>(bNodeList.size()));
  }
  // Get BNode value
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
  // Resize Array
  void resize(std::size_t index) {
    bNodeList.resize(index + 1);
    for (auto &entry : bNodeList) {
      if (entry.isEmpty()) {
        entry = BNode::make<Hole>();
      }
    }
  }

private:
  std::vector<BNode> bNodeList;
};
} // namespace Bencode_Lib