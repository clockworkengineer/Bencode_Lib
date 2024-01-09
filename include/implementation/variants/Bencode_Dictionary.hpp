#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace Bencode_Lib {

struct Dictionary : Variant {
  // Dictionary entry
  struct Entry {
    Entry(const std::string &key, BNode &bNode)
        : entryKey(key), entryBNode(std::move(bNode)) {}
    Entry(const std::string &key, BNode &&bNode)
        : entryKey(key), entryBNode(std::move(bNode)) {}
    std::string &getKey() { return (entryKey); }
    const std::string &getKey() const { return (entryKey); }
    BNode &getBNode() { return (entryBNode); }
    const BNode &getBNode() const { return (entryBNode); }

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
    auto key = entry.getKey();
    auto it = std::find_if(
        bNodeDictionary.begin(), bNodeDictionary.end(),
        [&key](const Entry &entry) -> bool { return (entry.getKey() > key); });
    bNodeDictionary.insert(it, std::move(entry));
  }
  void add(Entry &&entry) {
    auto key = entry.getKey();
    auto it = std::find_if(
        bNodeDictionary.begin(), bNodeDictionary.end(),
        [&key](const Entry &entry) -> bool { return (entry.getKey() > key); });
    bNodeDictionary.insert(it, std::move(entry));
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    if (auto it = std::find_if(bNodeDictionary.begin(), bNodeDictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.getKey() == key);
                               });
        it != bNodeDictionary.end()) {
      return (true);
    }
    return (false);
  }
  [[nodiscard]] int size() const {
    return (static_cast<int>(bNodeDictionary.size()));
  }
  BNode &operator[](const std::string &key) {
    if (auto it = std::find_if(bNodeDictionary.begin(), bNodeDictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.getKey() == key);
                               });
        it != bNodeDictionary.end()) {
      return (it->getBNode());
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  const BNode &operator[](const std::string &key) const {
    if (auto it = std::find_if(bNodeDictionary.begin(), bNodeDictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.getKey() == key);
                               });
        it != bNodeDictionary.end()) {
      return (it->getBNode());
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  [[nodiscard]] std::vector<Entry> &value() { return (bNodeDictionary); }
  [[nodiscard]] const std::vector<Entry> &value() const {
    return (bNodeDictionary);
  }

private:
  std::vector<Entry> bNodeDictionary;
};
} // namespace Bencode_Lib