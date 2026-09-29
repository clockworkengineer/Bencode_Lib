// File: Bencode_Serialization.hpp
//
// Description: Modern C++23 concept-based object mapping and serialization for Bencode_Lib.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_View.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace Bencode_Lib {

// ============================================================================
// Built-in Serializers & Deserializers for Primitives & Standard Types
// ============================================================================

// 1. Integral types (except bool)
template <typename T>
  requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
inline void to_bencode(Node &node, const T &val) {
  node = Node(static_cast<int64_t>(val));
}

template <typename T>
  requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
inline void from_bencode(const Node &node, T &val) {
  auto opt = node.as_int();
  if (!opt) {
    throw Node::Error("Node is not an integer.");
  }
  val = static_cast<T>(*opt);
}

template <typename T>
  requires(std::is_integral_v<T> && !std::is_same_v<T, bool>)
inline void from_bencode(const NodeView &view, T &val) {
  auto opt = view.as_int();
  if (!opt) {
    throw Node::Error("NodeView is not an integer.");
  }
  val = static_cast<T>(*opt);
}

// 2. Boolean
inline void to_bencode(Node &node, bool val) {
  node = Node(static_cast<int64_t>(val ? 1 : 0));
}

inline void from_bencode(const Node &node, bool &val) {
  auto opt = node.as_int();
  if (!opt) {
    throw Node::Error("Node is not an integer boolean.");
  }
  val = (*opt != 0);
}

inline void from_bencode(const NodeView &view, bool &val) {
  auto opt = view.as_int();
  if (!opt) {
    throw Node::Error("NodeView is not an integer boolean.");
  }
  val = (*opt != 0);
}

// 3. Floating point (encoded as truncated int64_t in standard Bencode)
template <typename T>
  requires(std::is_floating_point_v<T>)
inline void to_bencode(Node &node, const T &val) {
  node = Node(static_cast<long long>(val));
}

template <typename T>
  requires(std::is_floating_point_v<T>)
inline void from_bencode(const Node &node, T &val) {
  auto opt = node.as_int();
  if (!opt) {
    throw Node::Error("Node is not an integer for float conversion.");
  }
  val = static_cast<T>(*opt);
}

template <typename T>
  requires(std::is_floating_point_v<T>)
inline void from_bencode(const NodeView &view, T &val) {
  auto opt = view.as_int();
  if (!opt) {
    throw Node::Error("NodeView is not an integer for float conversion.");
  }
  val = static_cast<T>(*opt);
}

// 4. std::string
inline void to_bencode(Node &node, const std::string &val) {
  node = Node(val);
}

inline void from_bencode(const Node &node, std::string &val) {
  auto opt = node.as_string();
  if (!opt) {
    throw Node::Error("Node is not a string.");
  }
  val = std::string(*opt);
}

inline void from_bencode(const NodeView &view, std::string &val) {
  auto opt = view.as_string();
  if (!opt) {
    throw Node::Error("NodeView is not a string.");
  }
  val = std::string(*opt);
}

// 5. std::string_view
inline void to_bencode(Node &node, std::string_view val) {
  node = Node(std::string(val));
}

inline void from_bencode(const Node &node, std::string_view &val) {
  auto opt = node.as_string();
  if (!opt) {
    throw Node::Error("Node is not a string.");
  }
  val = *opt;
}

inline void from_bencode(const NodeView &view, std::string_view &val) {
  auto opt = view.as_string();
  if (!opt) {
    throw Node::Error("NodeView is not a string.");
  }
  val = *opt;
}

// 6. std::vector<std::byte> (Binary byte data)
inline void to_bencode(Node &node, const std::vector<std::byte> &val) {
  node = Node(std::string(reinterpret_cast<const char *>(val.data()), val.size()));
}

inline void from_bencode(const Node &node, std::vector<std::byte> &val) {
  auto opt = node.as_binary();
  if (!opt) {
    throw Node::Error("Node is not a binary byte string.");
  }
  val.assign(opt->begin(), opt->end());
}

inline void from_bencode(const NodeView &view, std::vector<std::byte> &val) {
  auto opt = view.as_binary();
  if (!opt) {
    throw Node::Error("NodeView is not a binary byte string.");
  }
  val.assign(opt->begin(), opt->end());
}

// 7. Forward declaration of detail namespace for containers
namespace detail {

template <typename T>
void to_bencode_adl(Node &node, const T &val);

template <typename NodeType, typename T>
void from_bencode_adl(const NodeType &node, T &val);

} // namespace detail

// 8. std::vector<T> (Lists)
template <typename T>
  requires(!std::is_same_v<T, std::byte> && !std::is_same_v<T, char>)
inline void to_bencode(Node &node, const std::vector<T> &val) {
  node = Node::make<List>();
  for (const auto &item : val) {
    Node child;
    detail::to_bencode_adl(child, item);
    NRef<List>(node).add(std::move(child));
  }
}

template <typename T>
  requires(!std::is_same_v<T, std::byte> && !std::is_same_v<T, char>)
inline void from_bencode(const Node &node, std::vector<T> &val) {
  if (!isA<List>(node)) {
    throw Node::Error("Node is not a list.");
  }
  val.clear();
  const auto &list = NRef<List>(node).value();
  val.reserve(list.size());
  for (const auto &elem : list) {
    T item{};
    detail::from_bencode_adl(elem, item);
    val.push_back(std::move(item));
  }
}

template <typename T>
  requires(!std::is_same_v<T, std::byte> && !std::is_same_v<T, char>)
inline void from_bencode(const NodeView &view, std::vector<T> &val) {
  auto listOpt = view.as_list();
  if (!listOpt) {
    throw Node::Error("NodeView is not a list.");
  }
  val.clear();
  val.reserve(listOpt->size());
  for (const auto &elem : *listOpt) {
    T item{};
    detail::from_bencode_adl(elem, item);
    val.push_back(std::move(item));
  }
}

// 9. std::map<std::string, T> (Dictionaries)
template <typename T>
inline void to_bencode(Node &node, const std::map<std::string, T> &val) {
  node = Node::make<Dictionary>();
  for (const auto &[k, v] : val) {
    Node child;
    detail::to_bencode_adl(child, v);
    NRef<Dictionary>(node).add(Dictionary::Entry(k, std::move(child)));
  }
}

template <typename T>
inline void from_bencode(const Node &node, std::map<std::string, T> &val) {
  if (!isA<Dictionary>(node)) {
    throw Node::Error("Node is not a dictionary.");
  }
  val.clear();
  const auto &dict = NRef<Dictionary>(node).value();
  for (const auto &entry : dict) {
    T item{};
    detail::from_bencode_adl(entry.getNode(), item);
    val[std::string(entry.getKey())] = std::move(item);
  }
}

template <typename T>
inline void from_bencode(const NodeView &view, std::map<std::string, T> &val) {
  auto dictOpt = view.as_dict();
  if (!dictOpt) {
    throw Node::Error("NodeView is not a dictionary.");
  }
  val.clear();
  for (const auto &[k, v] : *dictOpt) {
    T item{};
    detail::from_bencode_adl(v, item);
    val[std::string(k)] = std::move(item);
  }
}

// 10. std::optional<T>
template <typename T>
inline void to_bencode(Node &node, const std::optional<T> &val) {
  if (val.has_value()) {
    detail::to_bencode_adl(node, *val);
  }
}

template <typename T>
inline void from_bencode(const Node &node, std::optional<T> &val) {
  T item{};
  detail::from_bencode_adl(node, item);
  val = std::move(item);
}

template <typename T>
inline void from_bencode(const NodeView &view, std::optional<T> &val) {
  T item{};
  detail::from_bencode_adl(view, item);
  val = std::move(item);
}

// ============================================================================
// ADL Dispatch & Child Lookup Details
// ============================================================================

namespace detail {

template <typename T>
inline void to_bencode_adl(Node &node, const T &val) {
  using Bencode_Lib::to_bencode;
  to_bencode(node, val);
}

template <typename NodeType, typename T>
inline void from_bencode_adl(const NodeType &node, T &val) {
  using Bencode_Lib::from_bencode;
  from_bencode(node, val);
}

inline const Node *find_child(const Node &n, std::string_view key) {
  if (isA<Dictionary>(n) && n.contains(key)) {
    return &n.at(key);
  }
  return nullptr;
}

inline std::optional<NodeView> find_child(const NodeView &n, std::string_view key) {
  return n.get(key);
}

} // namespace detail

// ============================================================================
// C++23 Serialization Concepts
// ============================================================================

template <typename T>
concept BencodeSerializable = requires(Node &node, const T &val) {
  detail::to_bencode_adl(node, val);
};

template <typename T>
concept BencodeDeserializable = requires(const Node &node, T &val) {
  detail::from_bencode_adl(node, val);
};

template <typename T>
concept ViewBencodeDeserializable = requires(const NodeView &view, T &val) {
  detail::from_bencode_adl(view, val);
};

// ============================================================================
// Field Conversion Helpers for Struct Mappings
// ============================================================================

template <typename T>
inline void to_bencode_field(Node &n, std::string_view key, const std::optional<T> &val) {
  if (val.has_value()) {
    Node child;
    detail::to_bencode_adl(child, *val);
    if (!isA<Dictionary>(n)) {
      n = Node::make<Dictionary>();
    }
    NRef<Dictionary>(n).add(Dictionary::Entry(std::string(key), std::move(child)));
  }
}

template <typename T>
inline void to_bencode_field(Node &n, std::string_view key, const T &val) {
  Node child;
  detail::to_bencode_adl(child, val);
  if (!isA<Dictionary>(n)) {
    n = Node::make<Dictionary>();
  }
  NRef<Dictionary>(n).add(Dictionary::Entry(std::string(key), std::move(child)));
}

template <typename NodeType, typename T>
inline void from_bencode_field(const NodeType &n, std::string_view key, std::optional<T> &val) {
  auto child = detail::find_child(n, key);
  if (!child) {
    val = std::nullopt;
    return;
  }
  T item{};
  if constexpr (std::is_same_v<NodeType, Node>) {
    detail::from_bencode_adl(*child, item);
  } else {
    detail::from_bencode_adl(*child, item);
  }
  val = std::move(item);
}

template <typename NodeType, typename T>
inline void from_bencode_field(const NodeType &n, std::string_view key, T &val) {
  auto child = detail::find_child(n, key);
  if (!child) {
    throw Node::Error("Missing required dictionary key: " + std::string(key));
  }
  if constexpr (std::is_same_v<NodeType, Node>) {
    detail::from_bencode_adl(*child, val);
  } else {
    detail::from_bencode_adl(*child, val);
  }
}

// ============================================================================
// High-Level Conversion APIs
// ============================================================================

template <typename T>
inline T from_node(const Node &node) {
  T val{};
  detail::from_bencode_adl(node, val);
  return val;
}

template <typename T>
inline T from_node(const NodeView &view) {
  T val{};
  detail::from_bencode_adl(view, val);
  return val;
}

template <typename T>
inline Node to_node(const T &obj) {
  Node n;
  detail::to_bencode_adl(n, obj);
  return n;
}

} // namespace Bencode_Lib

// ============================================================================
// Macro Implementation: Variadic For-Each
// ============================================================================

#define BENCODE_EXPAND(x) x

#define BENCODE_GET_ARG_COUNT_IMPL( \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, N, ...) N

#define BENCODE_GET_ARG_COUNT(...) \
  BENCODE_EXPAND(BENCODE_GET_ARG_COUNT_IMPL(__VA_ARGS__, \
    BENCODE_FE_32, BENCODE_FE_31, BENCODE_FE_30, \
    BENCODE_FE_29, BENCODE_FE_28, BENCODE_FE_27, BENCODE_FE_26, \
    BENCODE_FE_25, BENCODE_FE_24, BENCODE_FE_23, BENCODE_FE_22, \
    BENCODE_FE_21, BENCODE_FE_20, BENCODE_FE_19, BENCODE_FE_18, \
    BENCODE_FE_17, BENCODE_FE_16, BENCODE_FE_15, BENCODE_FE_14, \
    BENCODE_FE_13, BENCODE_FE_12, BENCODE_FE_11, BENCODE_FE_10, \
    BENCODE_FE_9, BENCODE_FE_8, BENCODE_FE_7, BENCODE_FE_6, \
    BENCODE_FE_5, BENCODE_FE_4, BENCODE_FE_3, BENCODE_FE_2, \
    BENCODE_FE_1))

#define BENCODE_FE_1(M, x) M(x)
#define BENCODE_FE_2(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_1(M, __VA_ARGS__))
#define BENCODE_FE_3(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_2(M, __VA_ARGS__))
#define BENCODE_FE_4(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_3(M, __VA_ARGS__))
#define BENCODE_FE_5(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_4(M, __VA_ARGS__))
#define BENCODE_FE_6(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_5(M, __VA_ARGS__))
#define BENCODE_FE_7(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_6(M, __VA_ARGS__))
#define BENCODE_FE_8(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_7(M, __VA_ARGS__))
#define BENCODE_FE_9(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_8(M, __VA_ARGS__))
#define BENCODE_FE_10(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_9(M, __VA_ARGS__))
#define BENCODE_FE_11(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_10(M, __VA_ARGS__))
#define BENCODE_FE_12(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_11(M, __VA_ARGS__))
#define BENCODE_FE_13(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_12(M, __VA_ARGS__))
#define BENCODE_FE_14(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_13(M, __VA_ARGS__))
#define BENCODE_FE_15(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_14(M, __VA_ARGS__))
#define BENCODE_FE_16(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_15(M, __VA_ARGS__))
#define BENCODE_FE_17(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_16(M, __VA_ARGS__))
#define BENCODE_FE_18(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_17(M, __VA_ARGS__))
#define BENCODE_FE_19(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_18(M, __VA_ARGS__))
#define BENCODE_FE_20(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_19(M, __VA_ARGS__))
#define BENCODE_FE_21(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_20(M, __VA_ARGS__))
#define BENCODE_FE_22(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_21(M, __VA_ARGS__))
#define BENCODE_FE_23(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_22(M, __VA_ARGS__))
#define BENCODE_FE_24(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_23(M, __VA_ARGS__))
#define BENCODE_FE_25(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_24(M, __VA_ARGS__))
#define BENCODE_FE_26(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_25(M, __VA_ARGS__))
#define BENCODE_FE_27(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_26(M, __VA_ARGS__))
#define BENCODE_FE_28(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_27(M, __VA_ARGS__))
#define BENCODE_FE_29(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_28(M, __VA_ARGS__))
#define BENCODE_FE_30(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_29(M, __VA_ARGS__))
#define BENCODE_FE_31(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_30(M, __VA_ARGS__))
#define BENCODE_FE_32(M, x, ...) M(x) BENCODE_EXPAND(BENCODE_FE_31(M, __VA_ARGS__))

#define BENCODE_FOR_EACH_HELPER(action, ...) \
  BENCODE_EXPAND(action(__VA_ARGS__))

#define BENCODE_FOR_EACH(M, ...) \
  BENCODE_FOR_EACH_HELPER(BENCODE_GET_ARG_COUNT(__VA_ARGS__), M, __VA_ARGS__)

// ============================================================================
// Public Macros for Ergonomic Struct Mapping
// ============================================================================

#define BENCODE_TO_FIELD(field) ::Bencode_Lib::to_bencode_field(node, #field, val.field);
#define BENCODE_FROM_FIELD(field) ::Bencode_Lib::from_bencode_field(node, #field, val.field);

#define BENCODE_DEFINE_TYPE_NON_INTRUSIVE(Type, ...) \
  inline void to_bencode(::Bencode_Lib::Node &node, const Type &val) { \
    node = ::Bencode_Lib::Node::make<::Bencode_Lib::Dictionary>(); \
    BENCODE_FOR_EACH(BENCODE_TO_FIELD, __VA_ARGS__) \
  } \
  template <typename NodeType> \
  inline void from_bencode(const NodeType &node, Type &val) { \
    BENCODE_FOR_EACH(BENCODE_FROM_FIELD, __VA_ARGS__) \
  }

#define BENCODE_PAIR_FIRST(a, b) a
#define BENCODE_PAIR_SECOND(a, b) b

#define BENCODE_GET_FIRST(pair) BENCODE_EXPAND(BENCODE_PAIR_FIRST pair)
#define BENCODE_GET_SECOND(pair) BENCODE_EXPAND(BENCODE_PAIR_SECOND pair)

#define BENCODE_TO_PAIR(pair) ::Bencode_Lib::to_bencode_field(node, BENCODE_GET_SECOND(pair), val.BENCODE_GET_FIRST(pair));
#define BENCODE_FROM_PAIR(pair) ::Bencode_Lib::from_bencode_field(node, BENCODE_GET_SECOND(pair), val.BENCODE_GET_FIRST(pair));

#define BENCODE_STRUCT(Type, ...) \
  inline void to_bencode(::Bencode_Lib::Node &node, const Type &val) { \
    node = ::Bencode_Lib::Node::make<::Bencode_Lib::Dictionary>(); \
    BENCODE_FOR_EACH(BENCODE_TO_PAIR, __VA_ARGS__) \
  } \
  template <typename NodeType> \
  inline void from_bencode(const NodeType &node, Type &val) { \
    BENCODE_FOR_EACH(BENCODE_FROM_PAIR, __VA_ARGS__) \
  }

namespace Bencode_Lib {

// Node member definitions
template <typename T>
inline T Node::get() const {
  return from_node<T>(*this);
}

template <typename T>
inline T Node::as() const {
  return from_node<T>(*this);
}

template <typename T>
inline Node Node::from_object(const T &obj) {
  return to_node(obj);
}

// NodeView member definitions
template <typename T>
inline T NodeView::as() const {
  return from_node<T>(*this);
}

template <typename T>
inline T NodeView::get() const {
  return from_node<T>(*this);
}

// BencodeView member definitions
template <typename T>
inline T BencodeView::as() const {
  return rootNode.as<T>();
}

template <typename T>
inline T BencodeView::get() const {
  return rootNode.as<T>();
}

// Bencode member definitions
template <typename T>
inline T Bencode::get() const {
  return root().get<T>();
}

template <typename T>
inline T Bencode::as() const {
  return root().as<T>();
}

template <typename T>
inline Bencode Bencode::from_object(const T &obj) {
  Bencode b;
  b.root() = Node::from_object(obj);
  return b;
}

#if BENCODE_ENABLE_EXCEPTIONS
template <typename T>
inline T Bencode::parse_object(std::string_view bencodeString) {
  BencodeView view = BencodeView::parse(bencodeString);
  return view.get<T>();
}
#endif

} // namespace Bencode_Lib
