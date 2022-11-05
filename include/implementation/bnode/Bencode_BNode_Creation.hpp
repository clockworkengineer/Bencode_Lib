#pragma once
//
// C++ STL
//
#include <string>
#include <memory>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ==============
// BNode Creation
// ==============
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
// =====================
// BNode index overloads
// =====================
// List
inline BNode &BNode::operator[](int index) {
  return (BRef<List>(*this)[index]);
}
inline const BNode &BNode::operator[](int index) const
{
  return (BRef<const List>(*this)[index]);
}
// Dictionary
inline BNode &BNode::operator[](const std::string &key) {
  return (BRef<Dictionary>(*this)[key]);
}
inline const BNode &
BNode::operator[](const std::string &key) const
{
  return (BRef<const Dictionary>(*this)[key]);
}
} // namespace Bencode_Lib