#pragma once

namespace Bencode_Lib {
// List
inline Node &Node::operator[](const int index) {
  try {
    if (isA<Hole>(*this)) {
      *this = make<List>();
    }
    return BRef<List>(*this)[index];
  } catch ([[maybe_unused]] const Error &error) {
    BRef<List>(*this).resize(index);
    return BRef<List>(*this)[index];
  }
}
inline const Node &Node::operator[](const int index) const {
  return BRef<const List>(*this)[index];
}
// Dictionary
inline Node &Node::operator[](const std::string_view &key) {
  if (isA<Hole>(*this)) {
    *this = make<Dictionary>();
    BRef<Dictionary>(*this).add(Dictionary::Entry(key, make<Hole>()));
    return BRef<Dictionary>(*this).value().back().getNode();
  }
  return BRef<Dictionary>(*this)[key];
}
inline const Node &Node::operator[](const std::string_view &key) const {
  return BRef<const Dictionary>(*this)[key];
}
} // namespace Bencode_Lib