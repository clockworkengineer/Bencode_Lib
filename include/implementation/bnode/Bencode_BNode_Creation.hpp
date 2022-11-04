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