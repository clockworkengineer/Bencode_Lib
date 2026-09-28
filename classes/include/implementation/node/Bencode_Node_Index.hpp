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

template <typename T>
inline const T* Node::try_get(std::string_view key) const noexcept {
  if (!contains(key)) {
    return nullptr;
  }
  const Node& n = (*this)[key];
  if (isA<T>(n)) {
    return &NRef<const T>(n);
  }
  return nullptr;
}

template <typename T>
inline T* Node::try_get(std::string_view key) noexcept {
  if (!contains(key)) {
    return nullptr;
  }
  Node& n = (*this)[key];
  if (isA<T>(n)) {
    return &NRef<T>(n);
  }
  return nullptr;
}

template <typename T>
inline const T* Node::try_get(std::size_t index) const noexcept {
  if (isEmpty() || !isA<List>(*this)) {
    return nullptr;
  }
  const auto &list = NRef<const List>(*this);
  if (index >= list.value().size()) {
    return nullptr;
  }
  const Node &n = list.value()[index];
  if (isA<T>(n)) {
    return &NRef<const T>(n);
  }
  return nullptr;
}

template <typename T>
inline T* Node::try_get(std::size_t index) noexcept {
  if (isEmpty() || !isA<List>(*this)) {
    return nullptr;
  }
  auto &list = NRef<List>(*this);
  if (index >= list.value().size()) {
    return nullptr;
  }
  Node &n = list.value()[index];
  if (isA<T>(n)) {
    return &NRef<T>(n);
  }
  return nullptr;
}

inline std::optional<std::string_view> Node::get_string(std::string_view key) const noexcept {
  if (const auto *str = try_get<String>(key)) {
    return str->value();
  }
  return std::nullopt;
}

inline std::optional<int64_t> Node::get_int(std::string_view key) const noexcept {
  if (const auto *num = try_get<Integer>(key)) {
    return num->value();
  }
  return std::nullopt;
}

inline std::string_view Node::value_or(std::string_view key, std::string_view fallback) const noexcept {
  if (const auto *str = try_get<String>(key)) {
    return str->value();
  }
  return fallback;
}

inline int64_t Node::value_or(std::string_view key, int64_t fallback) const noexcept {
  if (const auto *num = try_get<Integer>(key)) {
    return num->value();
  }
  return fallback;
}

inline std::optional<std::string_view> Node::as_string() const noexcept {
  if (!isEmpty() && isA<String>(*this)) {
    return NRef<const String>(*this).value();
  }
  return std::nullopt;
}

inline std::optional<int64_t> Node::as_int() const noexcept {
  if (!isEmpty() && isA<Integer>(*this)) {
    return NRef<const Integer>(*this).value();
  }
  return std::nullopt;
}

} // namespace Bencode_Lib