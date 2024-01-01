#pragma once

#include <string>
#include <memory>
#include <variant>

namespace Bencode_Lib {

// Construct BNode from raw values
template <typename T> BNode::BNode(T value) {

  if constexpr (std::is_integral_v<T>) {
    *this = BNode::make<Integer>(value);
  } else if constexpr (std::is_floating_point_v<T>) {
    *this = BNode::make<Integer>(static_cast<long long>(value));
  } else if constexpr (std::is_null_pointer_v<T>) {
    *this = BNode::make<Integer>(0);
  } else if constexpr (std::is_same_v<T, const char *>) {
    *this = BNode::make<String>(value);
  } else if constexpr (std::is_same_v<T, std::string>) {
    *this = BNode::make<String>(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    bNodeVariant = std::move(value);
  }
}
// Construct BNode Array from initializer list
inline BNode::BNode(const Bencode::ListInitializer &list) {
  *this = BNode::make<List>();
  for (const auto &entry : list) {
    BRef<List>(*this).add(internalTypeToBNode(entry));
  }
}
// Construct BNode Dictionary from initializer list
inline BNode::BNode(const Bencode::DictionaryInitializer &object) {
  *this = BNode::make<Dictionary>();
  for (const auto &entry : object) {
    BRef<Dictionary>(*this).add(
        Dictionary::Entry(entry.first, internalTypeToBNode(entry.second)));
  }
}
// List
inline BNode &BNode::operator[](int index) {
  try {
    if (this->isHole()) { *this = BNode::make<List>(); }
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

inline BNode BNode::internalTypeToBNode(const Bencode::InternalType &type) {
  if (auto pValue = std::get_if<int>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<long>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<long long>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<float>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<double>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<long double>(&type)) { return (BNode(*pValue)); }
  if (auto pValue = std::get_if<std::string>(&type)) { return (BNode((*pValue))); }
  if (auto pValue = std::get_if<bool>(&type)) { return (BNode((*pValue))); }
  if ([[maybe_unused]] auto pValue = std::get_if<std::nullptr_t>(&type)) { return (BNode(0)); }
  if (auto pValue = std::get_if<BNode>(&type)) { return (std::move(*const_cast<BNode *>(pValue))); }
  throw Error("BNode of non-existant type could not be created.");
}

} // namespace Bencode_Lib