#pragma once

#include <string>

namespace Bencode_Lib {

// List
inline BNode &BNode::operator[](int index) {
  try {
    if (this->isHole()) {
      *this = BNode::make<List>();
    }
    return (BRef<List>(*this)[index]);
  } catch ([[maybe_unused]] const BNode::Error &error) {
    BRef<List>(*this).resize(index);
    return (BRef<List>(*this)[index]);
  }
}
inline const BNode &BNode::operator[](int index) const {
  return (BRef<const List>(*this)[index]);
}
// Dictionary
inline BNode &BNode::operator[](const std::string &key) {
  if (this->isHole()) {
    *this = BNode::make<Dictionary>();
    BRef<Dictionary>(*this).add(Dictionary::Entry(key, BNode::make<Hole>()));
    return (BRef<Dictionary>(*this).value().back().getBNode());
  }
  return (BRef<Dictionary>(*this)[key]);
}
inline const BNode &BNode::operator[](const std::string &key) const {
  return (BRef<const Dictionary>(*this)[key]);
}

} // namespace Bencode_Lib