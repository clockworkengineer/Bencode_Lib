#pragma once
//
// C++ STL
//
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace Bencode_Lib {
// =============
// Bencode Error
// =============
struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
// =====
// BNode
// =====
struct Variant;
struct BNode {
  enum class Type { base = 0, dictionary, list, integer, string };
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  BNode() = default;
  explicit BNode(std::unique_ptr<Variant> bNodeVariant)
      : m_bNodeVariant(std::move(bNodeVariant)) {}
  BNode(const BNode &other) = delete;
  BNode &operator=(const BNode &other) = delete;
  BNode(BNode &&other) = default;
  BNode &operator=(BNode &&other) = default;
  ~BNode() = default;
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  [[nodiscard]] Type getNodeType() const;
  // Get reference to BNode variant
  [[nodiscard]] std::unique_ptr<Variant> &getVariant();
  [[nodiscard]] const std::unique_ptr<Variant> &getVariant() const;

private:
  std::unique_ptr<Variant> m_bNodeVariant;
};
// ========
// Variants
// ========
struct Variant {
  explicit Variant(BNode::Type nodeType = BNode::Type::base)
      : m_nodeType(nodeType) {}
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  [[nodiscard]] BNode::Type getNodeType() const { return (m_nodeType); }

private:
  BNode::Type m_nodeType;
};
// ==========
// Dictionary
// ==========
struct Dictionary : Variant {
  using Entry = std::pair<std::string, BNode>;
  using EntryList = std::vector<Entry>;
  explicit Dictionary(Dictionary::EntryList &entryList)
      : Variant(BNode::Type::dictionary), m_dictionary(std::move(entryList)) {}
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
// ====
// List
// ====
struct List : Variant {
  using Entry = BNode;
  using EntryList = std::vector<Entry>;
  explicit List(EntryList &entryList)
      : Variant(BNode::Type::list), m_list(std::move(entryList)) {}
  [[nodiscard]] int size() const { return (static_cast<int>(m_list.size())); }
  [[nodiscard]] EntryList &list() { return (m_list); }
  [[nodiscard]] const EntryList &list() const { return (m_list); }
  BNode &operator[](int index) {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }
  const BNode &operator[](int index) const {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }

private:
  EntryList m_list;
};
// =======
// Integer
// =======
struct Integer : Variant {
  explicit Integer(int64_t integer)
      : Variant(BNode::Type::integer), m_value(integer) {}
  [[nodiscard]] int64_t integer() const { return (m_value); }

private:
  int64_t m_value = 0;
};
// ======
// String
// ======
struct String : Variant {
  explicit String(std::string string)
      : Variant(BNode::Type::string), m_string(std::move(string)) {}
  [[nodiscard]] std::string &string() { return (m_string); }
  [[nodiscard]] const std::string &string() const { return (m_string); }

private:
  std::string m_string;
};
// ==============================
// BNode base reference converter
// ==============================
template <typename T> void CheckBNodeType(const Variant &bNodeVariant) {
  if constexpr (std::is_same_v<T, String>) {
    if (bNodeVariant.getNodeType() != BNode::Type::string) {
      throw BNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, Integer>) {
    if (bNodeVariant.getNodeType() != BNode::Type::integer) {
      throw BNode::Error("Node not an integer.");
    }
  } else if constexpr (std::is_same_v<T, List>) {
    if (bNodeVariant.getNodeType() != BNode::Type::list) {
      throw BNode::Error("Node not a list.");
    }
  } else if constexpr (std::is_same_v<T, Dictionary>) {
    if (bNodeVariant.getNodeType() != BNode::Type::dictionary) {
      throw BNode::Error("Node not a dictionary.");
    }
  }
}
template <typename T> T &BRef(BNode &bNode) {
  CheckBNodeType<T>(*bNode.getVariant());
  return (static_cast<T &>(*bNode.getVariant()));
}
template <typename T> const T &BRef(const BNode &bNode) {
  CheckBNodeType<T>(*bNode.getVariant());
  return (static_cast<const T &>(*bNode.getVariant()));
}
// =============
// Node Creation
// =============
inline BNode makeDictionary(Dictionary::EntryList &dictionary) {
  return (BNode{std::make_unique<Dictionary>(dictionary)});
}
inline BNode makeList(List::EntryList &list) {
  return (BNode{std::make_unique<List>(list)});
}
inline BNode makeString(std::string stringValue) {
  return (BNode{std::make_unique<String>(stringValue)});
}
inline BNode makeInteger(int64_t integer) {
  return (BNode{std::make_unique<Integer>(integer)});
}
} // namespace Bencode_Lib