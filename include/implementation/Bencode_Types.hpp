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
namespace BencodeLib {
// =============
// Bencode Error
// =============
struct Error : public std::runtime_error {
  explicit Error(const std::string &message)
      : std::runtime_error("Bencode Error: " + message) {}
};
// ==================
// Bencode Node Types
// ==================
enum class BNodeType { base = 0, dictionary, list, integer, string };
// ====
// Base
// ====
struct BNode {
  using Ptr = std::unique_ptr<BNode>;
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  explicit BNode(BNodeType nodeType = BNodeType::base) : m_nodeType(nodeType) {}
  // No BNode is deleted through its base class so omit and save space
  // from virtual function table.
  // virtual ~BNode() = default;
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  [[nodiscard]] BNodeType getNodeType() const { return (m_nodeType); }

private:
  BNodeType m_nodeType;
};
// ==========
// Dictionary
// ==========
struct BNodeDict : BNode {
  using Entry = std::pair<std::string, BNode::Ptr>;
  explicit BNodeDict(std::vector<BNodeDict::Entry> &dictionary)
      : BNode(BNodeType::dictionary), m_dictionary(std::move(dictionary)) {}
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
      return (*it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  const BNode &operator[](const std::string &key) const {
    if (auto it = std::find_if(m_dictionary.begin(), m_dictionary.end(),
                               [&key](const Entry &entry) -> bool {
                                 return (entry.first == key);
                               });
        it != m_dictionary.end()) {
      return (*it->second);
    }
    throw BNode::Error("Invalid key used in dictionary.");
  }
  [[nodiscard]] std::vector<Entry> &dictionary() { return (m_dictionary); }
  [[nodiscard]] const std::vector<Entry> &dictionary() const {
    return (m_dictionary);
  }

private:
  std::vector<BNodeDict::Entry> m_dictionary;
};
// ====
// List
// ====
struct BNodeList : BNode {
  explicit BNodeList(std::vector<BNode::Ptr> &list)
      : BNode(BNodeType::list), m_list(std::move(list)) {}
  [[nodiscard]] int size() const { return (static_cast<int>(m_list.size())); }
  [[nodiscard]] std::vector<BNode::Ptr> &list() { return (m_list); }
  [[nodiscard]] const std::vector<BNode::Ptr> &list() const { return (m_list); }
  BNode &operator[](int index) {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (*m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }
  const BNode &operator[](int index) const {
    if ((index >= 0) && (index < (static_cast<int>(m_list.size())))) {
      return (*m_list[index]);
    }
    throw BNode::Error("Invalid index used in list.");
  }

private:
  std::vector<BNode::Ptr> m_list;
};
// =======
// Integer
// =======
struct BNodeInteger : BNode {
  explicit BNodeInteger(int64_t integer)
      : BNode(BNodeType::integer), m_value(integer) {}
  [[nodiscard]] int64_t integer() const { return (m_value); }

private:
  int64_t m_value = 0;
};
// ======
// String
// ======
struct BNodeString : BNode {
  explicit BNodeString(std::string stringValue)
      : BNode(BNodeType::string), m_string(std::move(stringValue)) {}
  [[nodiscard]] std::string &string() { return (m_string); }
  [[nodiscard]] const std::string &string() const { return (m_string); }

private:
  std::string m_string;
};
// ==============================
// BNode base reference converter
// ==============================
template <typename T> void CheckBNodeType(const BNode &bNode) {
  if constexpr (std::is_same_v<T, BNodeString>) {
    if (bNode.getNodeType() != BNodeType::string) {
      throw BNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, BNodeInteger>) {
    if (bNode.getNodeType() != BNodeType::integer) {
      throw BNode::Error("Node not an integer.");
    }
  } else if constexpr (std::is_same_v<T, BNodeList>) {
    if (bNode.getNodeType() != BNodeType::list) {
      throw BNode::Error("Node not a list.");
    }
  } else if constexpr (std::is_same_v<T, BNodeDict>) {
    if (bNode.getNodeType() != BNodeType::dictionary) {
      throw BNode::Error("Node not a dictionary.");
    }
  }
}
template <typename T> T &BNodeRef(BNode &bNode) {
  CheckBNodeType<T>(bNode);
  return (static_cast<T &>(bNode));
}
template <typename T> const T &BNodeRef(const BNode &bNode) {
  CheckBNodeType<T>(bNode);
  return (static_cast<const T &>(bNode));
}
// ===============
// Index overloads
// ===============
inline BNode &BNode::operator[](const std::string &key) // Dictionary
{
  return (BNodeRef<BNodeDict>(*this)[key]);
}
inline BNode &BNode::operator[](int index) // List
{
  return (BNodeRef<BNodeList>(*this)[index]);
}
inline const BNode &
BNode::operator[](const std::string &key) const // Dictionary
{
  return (BNodeRef<const BNodeDict>(*this)[key]);
}
inline const BNode &BNode::operator[](int index) const // List
{
  return (BNodeRef<const BNodeList>(*this)[index]);
}
} // namespace BencodeLib