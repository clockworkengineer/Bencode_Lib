#pragma once

#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <algorithm>

namespace Bencode_Lib {

struct Dictionary : Variant {
  // Dictionary entry
  struct Entry {
    Entry(std::string key, BNode &bNode)
        : entryKey(std::move(key)), entryBNode(std::move(bNode)) {}
    Entry(std::string key, BNode &&bNode)
        : entryKey(std::move(key)), entryBNode(std::move(bNode)) {}
    std::string &getKey() { return entryKey; }
    [[nodiscard]] const std::string &getKey() const { return entryKey; }
    BNode &getBNode() { return entryBNode; }
    [[nodiscard]] const BNode &getBNode() const { return entryBNode; }

  private:
    std::string entryKey;
    BNode entryBNode;
  };
  // Constructors/Destructors
  Dictionary() : Variant(Type::dictionary) {}
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
    return std::ranges::find_if(bNodeDictionary,
        [&key](Entry &entry) -> bool { return entry.getKey() == key; });
  }
  [[nodiscard]] std::vector<Entry>::const_iterator
  findKey(const std::string &key) const {
    return std::ranges::find_if(
        bNodeDictionary,
        [&key](const Entry &entry) -> bool { return entry.getKey() == key; });
  }
  // Find place for entry in ordered dictionary
  std::vector<Entry>::iterator findEntry(const Entry &entry) {
    return std::ranges::find_if(bNodeDictionary,
                                [&entry](const Entry &current) -> bool {
                                  return current.getKey() >= entry.getKey();
                                });
  }
  std::vector<Entry> bNodeDictionary;
};
} // namespace Bencode_Lib