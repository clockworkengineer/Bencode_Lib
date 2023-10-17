#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace Bencode_Lib {

struct Dictionary : Variant {
  using Entry = std::pair<std::string, BNode>;
  using EntryList = std::vector<Entry>;
  explicit Dictionary(Dictionary::EntryList &entryList)
      : Variant(Variant::Type::dictionary), bNodeDictionary(std::move(entryList)) {}
  [[nodiscard]] bool contains(const std::string &key) const {
    if (auto it = std::find_if(bNodeDictionary.begin(), bNodeDictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
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
                                 return (entry.first == key);
                               });
        it != bNodeDictionary.end()) {
      return (it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  const BNode &operator[](const std::string &key) const {
    if (auto it = std::find_if(bNodeDictionary.begin(), bNodeDictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
                               });
        it != bNodeDictionary.end()) {
      return (it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  [[nodiscard]] EntryList &dictionary() { return (bNodeDictionary); }
  [[nodiscard]] const EntryList &dictionary() const { return (bNodeDictionary); }

private:
  Dictionary::EntryList bNodeDictionary;
};
} // namespace Bencode_Lib