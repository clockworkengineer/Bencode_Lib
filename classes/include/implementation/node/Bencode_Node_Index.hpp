#pragma once

namespace Bencode_Lib {
// List
inline Node &Node::operator[](const int index) {
  try {
    if (isA<Hole>(*this)) {
      *this = make<List>();
    }
    return NRef<List>(*this)[index];
  } catch ([[maybe_unused]] const Error &error) {
    NRef<List>(*this).resize(index);
    return NRef<List>(*this)[index];
  }
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
} // namespace Bencode_Lib