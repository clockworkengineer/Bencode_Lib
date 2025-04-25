#pragma once

namespace Bencode_Lib {

// List Error
struct ListError final : std::runtime_error {
  explicit ListError(std::string_view message)
      : std::runtime_error(std::string("List Error: ").append(message)) {}
};
struct List : Variant {
  using Error = ListError;
  using Entry = BNode;
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
  // Get BNode size
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeList.size());
  }
  // Get BNode value
  [[nodiscard]]ListEntries &value() { return bNodeList; }
  [[nodiscard]] const ListEntries &value() const { return bNodeList; }
  // Get BNode at index
  BNode &operator[](const int index) {
    validateIndex(index);
    return bNodeList[index];
  }
  const BNode &operator[](const int index) const {
    validateIndex(index);
    return bNodeList[index];
  }
  // Resize Array
  void resize(const std::size_t index) {
    bNodeList.resize(index + 1);
    for (auto &entry : bNodeList) {
      if (entry.isEmpty()) {
        entry = BNode::make<Hole>();
      }
    }
  }

private:
  // ensure the index is within the bounds
  void validateIndex(const int index) const {
    if (!(index >= 0 && index < static_cast<int>(bNodeList.size()))) {
      throw Error("Invalid index used in list.");
    }
  }

 ListEntries bNodeList {};
};
} // namespace Bencode_Lib