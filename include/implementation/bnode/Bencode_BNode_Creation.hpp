#pragma once

#include <string>
#include <memory>

namespace Bencode_Lib {

// List
inline BNode &BNode::operator[](int index) {
  return (BRef<List>(*this)[index]);
}
inline const BNode &BNode::operator[](int index) const {
  return (BRef<const List>(*this)[index]);
}
// Dictionary
inline BNode &BNode::operator[](const std::string &key) {
  return (BRef<Dictionary>(*this)[key]);
}
inline const BNode &BNode::operator[](const std::string &key) const {
  return (BRef<const Dictionary>(*this)[key]);
}

} // namespace Bencode_Lib