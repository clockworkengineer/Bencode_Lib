// File: Bencode_Node_Index.hpp
//
// Description: Utility functions for indexing into Bencode lists and dictionary nodes.
//

#pragma once

namespace Bencode_Lib {
// List
/// <summary>
/// Access list elements by index, creating a list if the node is currently empty.
/// </summary>
inline Node &Node::operator[](const std::size_t index) {
  if (isA<Hole>(*this)) {
    *this = make<List>();
  }
  return NRef<List>(*this)[index];
}
/// <summary>
/// Access list elements by index without modifying the node.
/// </summary>
inline const Node &Node::operator[](const std::size_t index) const {
  return NRef<const List>(*this)[index];
}
// Dictionary
/// <summary>
/// Access dictionary values by key, creating a dictionary if the node is empty.
/// </summary>
inline Node &Node::operator[](const std::string_view &key) {
  if (isA<Hole>(*this)) {
    *this = make<Dictionary>();
    NRef<Dictionary>(*this).add(Dictionary::Entry(key, make<Hole>()));
    return NRef<Dictionary>(*this).value().back().getNode();
  }
  return NRef<Dictionary>(*this)[key];
}
/// <summary>
/// Access dictionary values by key without modifying the node.
/// </summary>
inline const Node &Node::operator[](const std::string_view &key) const {
  return NRef<const Dictionary>(*this)[key];
}

/// <summary>
/// Determine whether the dictionary contains the specified key.
/// </summary>
inline bool Node::contains(std::string_view key) const noexcept {
  if (isEmpty()) {
    return false;
  }
  if (!isA<Dictionary>(*this)) {
    return false;
  }
  return NRef<const Dictionary>(*this).contains(key);
}

/// <summary>
/// Return a dictionary entry by key, throwing if the node is not a dictionary.
/// </summary>
inline Node &Node::at(std::string_view key) {
  if (!isA<Dictionary>(*this)) {
    throw Node::Error("Type error: Node is not a dictionary.");
  }
  return NRef<Dictionary>(*this).at(key);
}

/// <summary>
/// Return a constant dictionary entry by key, throwing if the node is not a dictionary.
/// </summary>
inline const Node &Node::at(std::string_view key) const {
  if (!isA<Dictionary>(*this)) {
    throw Node::Error("Type error: Node is not a dictionary.");
  }
  return NRef<const Dictionary>(*this).at(key);
}
} // namespace Bencode_Lib