// File: Bencode_Node_Index.hpp
//
// Description: Utility functions for indexing into Bencode lists and dictionary nodes.
//

#pragma once

namespace Bencode_Lib {
// List
inline Node &Node::operator[](const int index) {
  if (isA<Hole>(*this)) {
    *this = make<List>();
  }
  return NRef<List>(*this)[index];
}
inline const Node &Node::operator[](const int index) const {
  return NRef<const List>(*this)[index];
}
// Dictionary
inline Node &Node::operator[](const std::string_view &key) {
  if (isA<Hole>(*this)) {
    *this = make<Dictionary>();
    NRef<Dictionary>(*this).add(Dictionary::Entry(key, make<Hole>()));
    return NRef<Dictionary>(*this).value().back().getNode();
  }
  return NRef<Dictionary>(*this)[key];
}
inline const Node &Node::operator[](const std::string_view &key) const {
  return NRef<const Dictionary>(*this)[key];
}

inline bool Node::contains(std::string_view key) const noexcept {
  if (isEmpty()) {
    return false;
  }
  if (!isA<Dictionary>(*this)) {
    return false;
  }
  return NRef<const Dictionary>(*this).contains(key);
}

inline Node &Node::at(std::string_view key) {
  return NRef<Dictionary>(*this).at(key);
}

inline const Node &Node::at(std::string_view key) const {
  return NRef<const Dictionary>(*this).at(key);
}
} // namespace Bencode_Lib