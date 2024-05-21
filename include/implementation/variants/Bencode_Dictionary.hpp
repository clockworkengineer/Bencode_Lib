#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <algorithm>

namespace Bencode_Lib {

struct Dictionary : Variant {
  // Dictionary entry
  struct Entry {
    Entry(const std::string &key, BNode &bNode)
        : entryKey(key), entryBNode(std::move(bNode)) {}
    Entry(const std::string &key, BNode &&bNode)
        : entryKey(key), entryBNode(std::move(bNode)) {}
    std::string &getKey() { return entryKey; }
    const std::string &getKey() const { return entryKey; }
    BNode &getBNode() { return entryBNode; }
    const BNode &getBNode() const { return entryBNode; }

  private:
    std::string entryKey;
    BNode entryBNode;
  };
  // Constructors/Destructors
  Dictionary() : Variant(Variant::Type::dictionary) {}
  Dictionary(const Dictionary &other) = default;
  Dictionary &operator=(const Dictionary &other) = default;
  Dictionary(Dictionary &&other) = default;
  Dictionary &operator=(Dictionary &&other) = default;
  ~Dictionary() = default;
  // Add array element
  void add(Entry &entry) {
    bNodeDictionary.insert(findEntry(entry), std::move(entry));
  }
  void add(Entry &&entry) {
    bNodeDictionary.insert(findEntry(entry), std::move(entry));
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    const auto it = findKey(key);
    return it != bNodeDictionary.end();
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }
  BNode &operator[](const std::string &key) {
    if (const auto it = findKey(key); it != bNodeDictionary.end()) {
      return it->getBNode();
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  const BNode &operator[](const std::string &key) const {
    if (const auto it = findKey(key); it != bNodeDictionary.end()) {
      return it->getBNode();
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  [[nodiscard]] std::vector<Entry> &value() { return bNodeDictionary; }
  [[nodiscard]] const std::vector<Entry> &value() const {
    return bNodeDictionary;
  }

private:
  // Search for a given entry given a key
  [[nodiscard]] std::vector<Entry>::iterator findKey(const std::string &key) {
    const auto entry = std::ranges::find_if(bNodeDictionary,
        [&key](Entry &entry) -> bool { return entry.getKey() == key; });
    return entry;
  }
  [[nodiscard]] std::vector<Entry>::const_iterator
  findKey(const std::string &key) const {
    const auto entry = std::ranges::find_if(
        bNodeDictionary,
        [&key](const Entry &entry) -> bool { return entry.getKey() == key; });
    return entry;
  }
  // Find place for entry in ordered dictionary
  std::vector<Entry>::iterator findEntry(const Entry &entry) {
    const auto it = std::ranges::find_if(
        bNodeDictionary,
                           [&entry](const Entry &current) -> bool {
                             return current.getKey() >= entry.getKey();
                           });
    return it;
  }
  std::vector<Entry> bNodeDictionary;
};
} // namespace Bencode_Lib