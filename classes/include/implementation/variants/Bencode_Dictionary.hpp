#pragma once

#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <algorithm>

namespace Bencode_Lib {

// Dictionary error
struct DictionaryError final : std::runtime_error {
  explicit DictionaryError(const std::string &message)
      : std::runtime_error("Dictionary Error: " + message) {}
};

// Dictionary entry
struct DictionaryEntry {
  DictionaryEntry(std::string key, BNode &&bNode)
      : key(std::move(key)), bNode(std::move(bNode)) {}
  std::string &getKey() { return key; }
  [[nodiscard]] const std::string &getKey() const { return key; }
  BNode &getBNode() { return bNode; }
  [[nodiscard]] const BNode &getBNode() const { return bNode; }

private:
  std::string key;
  BNode bNode{};
};
// Dictionary variant
struct Dictionary : Variant {
  using Error = DictionaryError;
  using Entry = DictionaryEntry;
  // Constructors/Destructors
  Dictionary() : Variant(Type::dictionary) {}
  Dictionary(const Dictionary &other) = default;
  Dictionary &operator=(const Dictionary &other) = default;
  Dictionary(Dictionary &&other) = default;
  Dictionary &operator=(Dictionary &&other) = default;
  ~Dictionary() = default;
  // Add Entry to Dictionary
  template <typename T> void add(T &&entry) {
    bNodeDictionary.emplace_back(std::forward<T>(entry));
    std::ranges::sort(bNodeDictionary, [](const Entry &a, const Entry &b) {
      return a.getKey() < b.getKey();
    });
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    try {
      auto _ = findEntryWithKey(bNodeDictionary, key);
    } catch (const Error &error) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }

  BNode &operator[](const std::string &key) {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }
  const BNode &operator[](const std::string &key) const {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }

  [[nodiscard]] std::vector<Entry> &value() { return bNodeDictionary; }
  [[nodiscard]] const std::vector<Entry> &value() const {
    return bNodeDictionary;
  }

private:

  template <typename T>
  static auto findEntryByKey(T &dictionary, const std::string &key) {
    auto it =
        std::ranges::find_if(dictionary, [&key](auto &entry) -> bool {
          return entry.getKey() == key;
        });
    if (it == dictionary.end()) {
      throw Error("Invalid key used in dictionary.");
    }
    return it;
  }

  [[nodiscard]] static std::vector<Entry>::const_iterator
  findEntryWithKey(const std::vector<Entry> &dictionary,
                   const std::string &key) {
    return findEntryByKey(dictionary, key);
  }

  [[nodiscard]] static std::vector<Entry>::iterator
  findEntryWithKey(std::vector<Entry> &dictionary, const std::string &key) {
    return findEntryByKey(dictionary, key);
  }

  std::vector<Entry> bNodeDictionary;
};

} // namespace Bencode_Lib