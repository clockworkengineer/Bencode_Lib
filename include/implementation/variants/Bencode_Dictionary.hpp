#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <memory>
#include <utility>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ==========
// Dictionary
// ==========
struct Dictionary : Variant {
  using Entry = std::pair<std::string, BNode>;
  using EntryList = std::vector<Entry>;
  explicit Dictionary(Dictionary::EntryList &entryList)
      : Variant(Variant::Type::dictionary), m_dictionary(std::move(entryList)) {
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    if (auto it = std::find_if(m_dictionary.begin(), m_dictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
                               });
        it != m_dictionary.end()) {
      return (true);
    }
    return (false);
  }
  [[nodiscard]] int size() const {
    return (static_cast<int>(m_dictionary.size()));
  }
  BNode &operator[](const std::string &key) {
    if (auto it = std::find_if(m_dictionary.begin(), m_dictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
                               });
        it != m_dictionary.end()) {
      return (it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  const BNode &operator[](const std::string &key) const {
    if (auto it = std::find_if(m_dictionary.begin(), m_dictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
                               });
        it != m_dictionary.end()) {
      return (it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  [[nodiscard]] EntryList &dictionary() { return (m_dictionary); }
  [[nodiscard]] const EntryList &dictionary() const { return (m_dictionary); }

private:
  Dictionary::EntryList m_dictionary;
};
} // namespace Bencode_Lib