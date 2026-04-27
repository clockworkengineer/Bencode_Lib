#pragma once

#include "Bencode_FixedVector.hpp"

#include <algorithm>
#include <string>
#include <string_view>
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
#include <vector>
#endif

namespace Bencode_Lib {

// Dictionary entry
struct DictionaryEntry {
  DictionaryEntry() = default;

  template <typename Key>
  DictionaryEntry(Key &&key, Node &&bNode)
      : key(std::forward<Key>(key)), bNode(std::move(bNode)) {}

  [[nodiscard]] std::string_view getKey() const { return key; }
  [[nodiscard]] Node &getNode() { return bNode; }
  [[nodiscard]] const Node &getNode() const { return bNode; }

  std::string key;
  Node bNode{};
};

// Dictionary variant
struct Dictionary : Variant {
  using Entry = DictionaryEntry;
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
  using Entries = std::vector<Entry>;
#else
  using Entries = FixedVector<Entry, BENCODE_MAX_CONTAINER_SIZE>;
#endif
  // Constructors/Destructors
  Dictionary() : Variant(Type::dictionary) {}
  Dictionary(const Dictionary &other) = default;
  Dictionary &operator=(const Dictionary &other) = default;
  Dictionary(Dictionary &&other) = default;
  Dictionary &operator=(Dictionary &&other) = default;
  ~Dictionary() = default;
  // Add Entry to Dictionary
  template <typename T> void add(T &&entry) {
    const auto &key = entry.key;
    auto it =
        std::lower_bound(bNodeDictionary.begin(), bNodeDictionary.end(), key,
                         [](const Entry &lhs, const std::string_view rhsKey) {
                           return lhs.key < rhsKey;
                         });
    if (it != bNodeDictionary.end() && it->key == key) {
      throw Node::Error("Duplicate dictionary key.");
    }
    bNodeDictionary.insert(it, std::forward<T>(entry));
  }

  template <typename T> void appendSorted(T &&entry) {
    const auto &key = entry.key;
    if (!bNodeDictionary.empty()) {
      const auto &previousKey = bNodeDictionary.back().key;
      if (previousKey == key) {
        throw Node::Error("Duplicate dictionary key.");
      }
      if (previousKey > key) {
        throw Node::Error("Dictionary keys not in sequence.");
      }
    }
    bNodeDictionary.push_back(std::forward<T>(entry));
  }

  [[nodiscard]] bool contains(const std::string_view key) const {
    auto it =
        std::lower_bound(bNodeDictionary.begin(), bNodeDictionary.end(), key,
                         [](const Entry &lhs, const std::string_view rhsKey) {
                           return lhs.key < rhsKey;
                         });
    return it != bNodeDictionary.end() && it->key == key;
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }

  Node &operator[](const std::string_view key) {
    return findEntryWithKey(bNodeDictionary, key)->bNode;
  }
  const Node &operator[](const std::string_view key) const {
    return findEntryWithKey(bNodeDictionary, key)->bNode;
  }

  [[nodiscard]] Entries &value() { return bNodeDictionary; }
  [[nodiscard]] const Entries &value() const { return bNodeDictionary; }

private:
  template <typename T>
  static auto findEntry(T &dictionary, const std::string_view &key) {
    auto it =
        std::lower_bound(dictionary.begin(), dictionary.end(), key,
                         [](const Entry &lhs, const std::string_view rhsKey) {
                           return lhs.key < rhsKey;
                         });
    if (it == dictionary.end() || it->key != key) {
      throw Node::Error("Invalid key used in dictionary.");
    }
    return it;
  }

  static Entries::const_iterator findEntryWithKey(const Entries &dictionary,
                                                  const std::string_view &key) {
    return findEntry(dictionary, key);
  }

  static Entries::iterator findEntryWithKey(Entries &dictionary,
                                            const std::string_view key) {
    return findEntry(dictionary, key);
  }

  Entries bNodeDictionary{};
};

} // namespace Bencode_Lib