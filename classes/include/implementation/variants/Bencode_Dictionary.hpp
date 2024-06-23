#pragma once

#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <algorithm>

namespace Bencode_Lib {

struct Dictionary : Variant {
  // Dictionary Error
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("Dictionary Error: " + message) {}
  };
  // Dictionary entry
  struct Entry {
    Entry(std::string key, BNode &&bNode)
        : key(std::move(key)), bNode(std::move(bNode)) {}
    std::string &getKey() { return key; }
    [[nodiscard]] const std::string &getKey() const { return key; }
    BNode &getBNode() { return bNode; }
    [[nodiscard]] const BNode &getBNode() const { return bNode; }
  private:
    std::string key;
    BNode bNode{};
  };
  // Constructors/Destructors
  Dictionary() : Variant(Type::dictionary) {}
  Dictionary(const Dictionary &other) = default;
  Dictionary &operator=(const Dictionary &other) = default;
  Dictionary(Dictionary &&other) = default;
  Dictionary &operator=(Dictionary &&other) = default;
  ~Dictionary() = default;
  // Add Entry to Dictionary
  template<typename T>
  void add(T&& entry) {
    bNodeDictionary.emplace_back(std::forward<T>(entry));
    std::ranges::sort(bNodeDictionary, [](const Entry &a, const Entry &b) {
      return a.getKey() < b.getKey();
    });
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    const auto it = findKey(bNodeDictionary, key);
    return it != bNodeDictionary.end();
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }
  BNode &operator[](const std::string &key) {
    auto foundEntry = findEntryWithKey(key);
    return foundEntry->getBNode();
  }

  const BNode &operator[](const std::string &key) const {
    auto foundEntry = findEntryWithKey(key);
    return foundEntry->getBNode();
  }
  [[nodiscard]] std::vector<Entry> &value() { return bNodeDictionary; }
  [[nodiscard]] const std::vector<Entry> &value() const {
    return bNodeDictionary;
  }

private:

  static bool isKeyMatch(const Entry &entry, const std::string &key) {
    return entry.getKey() == key;
  }

  [[nodiscard]] static std::vector<Entry>::iterator
  findKey(std::vector<Entry> &dictionary, const std::string &key) {
    return std::ranges::find_if(dictionary, [&key](Entry &entry) -> bool {
      return isKeyMatch(entry, key);
    });
  }

  [[nodiscard]] static std::vector<Entry>::const_iterator
  findKey(const std::vector<Entry> &dictionary, const std::string &key) {
    return std::ranges::find_if(dictionary, [&key](const Entry &entry) -> bool {
      return isKeyMatch(entry, key);
    });
  }

  [[nodiscard]] std::vector<Entry>::const_iterator
  findEntryWithKey(const std::string &key) const {
    const auto it = findKey(bNodeDictionary, key);
    if (it == bNodeDictionary.end()) {
      throw Error("Invalid key used in dictionary.");
    }
    return it;
  }

  std::vector<Entry>::iterator
  findEntryWithKey(const std::string &key) {
    auto it = findKey(bNodeDictionary, key);
    if (it == bNodeDictionary.end()) {
      throw Error("Invalid key used in dictionary.");
    }
    return it;
  }
  std::vector<Entry> bNodeDictionary;
};

} // namespace Bencode_Lib