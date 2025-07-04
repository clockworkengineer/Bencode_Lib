#pragma once

namespace Bencode_Lib {

struct List : Variant {
  using Entry = Node;
  using ListEntries = std::vector<Entry>;
  // Constructors/Destructors
  List() : Variant(Type::list){}
  List(const List &other) = default;
  List &operator=(const List &other) = default;
  List(List &&other) = default;
  List &operator=(List &&other) = default;
  ~List() = default;
  // Add array element
  void add(Entry bNode) {
    bNodeList.emplace_back(std::move(bNode));
  }
  // Get Node size
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeList.size());
  }
  // Get Node value
  [[nodiscard]]ListEntries &value() { return bNodeList; }
  [[nodiscard]] const ListEntries &value() const { return bNodeList; }
  // Get Node at index
  Node &operator[](const int index) {
    validateIndex(index);
    return bNodeList[index];
  }
  const Node &operator[](const int index) const {
    validateIndex(index);
    return bNodeList[index];
  }
  // Resize Array
  void resize(const std::size_t index) {
    bNodeList.resize(index + 1);
    for (auto &entry : bNodeList) {
      if (entry.isEmpty()) {
        entry = Node::make<Hole>();
      }
    }
  }

private:
  // ensure the index is within the bounds
  void validateIndex(const int index) const {
    if (!(index >= 0 && index < static_cast<int>(bNodeList.size()))) {
      throw Node::Error("Invalid index used in list.");
    }
  }

 ListEntries bNodeList {};
};
} // namespace Bencode_Lib