// File: Bencode_NodeView.hpp
//
// Description: Non-owning zero-copy AST node view for Bencode payloads.
//

#pragma once

#include "Bencode_Config.hpp"
#include "Bencode_Status.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace Bencode_Lib {

class NodeView;
struct DictEntry;

class NodeView {
public:
  using IntegerType = int64_t;
  using StringType = std::string_view;
  using ListType = std::span<const NodeView>;
  using DictType = std::span<const DictEntry>;

  enum class Type : uint8_t {
    Empty = 0,
    Integer,
    String,
    List,
    Dictionary
  };

private:
  struct SpanData {
    const void *ptr{nullptr};
    std::size_t len{0};
  };

  Type nodeType{Type::Empty};
  union {
    int64_t intValue;
    std::string_view strValue;
    SpanData spanValue;
  };

public:
  constexpr NodeView() noexcept : nodeType(Type::Empty), intValue(0) {}
  constexpr explicit NodeView(int64_t val) noexcept : nodeType(Type::Integer), intValue(val) {}
  constexpr explicit NodeView(std::string_view val) noexcept : nodeType(Type::String), strValue(val) {}
  constexpr explicit NodeView(std::span<const NodeView> val) noexcept
      : nodeType(Type::List), spanValue{val.data(), val.size()} {}
  constexpr explicit NodeView(std::span<const DictEntry> val) noexcept;

  constexpr NodeView(const NodeView &other) noexcept = default;
  constexpr NodeView &operator=(const NodeView &other) noexcept = default;
  constexpr NodeView(NodeView &&other) noexcept = default;
  constexpr NodeView &operator=(NodeView &&other) noexcept = default;
  ~NodeView() = default;

  [[nodiscard]] constexpr Type type() const noexcept { return nodeType; }
  [[nodiscard]] constexpr bool empty() const noexcept { return nodeType == Type::Empty; }
  [[nodiscard]] constexpr bool is_integer() const noexcept { return nodeType == Type::Integer; }
  [[nodiscard]] constexpr bool is_string() const noexcept { return nodeType == Type::String; }
  [[nodiscard]] constexpr bool is_list() const noexcept { return nodeType == Type::List; }
  [[nodiscard]] constexpr bool is_dict() const noexcept { return nodeType == Type::Dictionary; }

  // Direct safe conversions for the current node
  [[nodiscard]] std::optional<int64_t> as_int() const noexcept {
    if (nodeType == Type::Integer) {
      return intValue;
    }
    return std::nullopt;
  }

  [[nodiscard]] std::optional<std::string_view> as_string() const noexcept {
    if (nodeType == Type::String) {
      return strValue;
    }
    return std::nullopt;
  }

  [[nodiscard]] std::optional<std::span<const std::byte>> as_binary() const noexcept {
    if (nodeType == Type::String) {
      return std::span<const std::byte>(
          reinterpret_cast<const std::byte *>(strValue.data()), strValue.size());
    }
    return std::nullopt;
  }

  [[nodiscard]] std::optional<ListType> as_list() const noexcept {
    if (nodeType == Type::List) {
      return ListType(static_cast<const NodeView *>(spanValue.ptr), spanValue.len);
    }
    return std::nullopt;
  }

  [[nodiscard]] std::optional<DictType> as_dict() const noexcept;

  // Object mapping template methods
  template <typename T>
  [[nodiscard]] T as() const;
  template <typename T>
  [[nodiscard]] T get() const;

  [[nodiscard]] std::size_t size() const noexcept {
    switch (nodeType) {
    case Type::String:
      return strValue.size();
    case Type::List:
    case Type::Dictionary:
      return spanValue.len;
    default:
      return 0;
    }
  }

  // Lookup in dictionary by key (binary search over sorted entries)
  [[nodiscard]] std::optional<NodeView> get(std::string_view key) const noexcept;

  // Lookup in list by index
  [[nodiscard]] std::optional<NodeView> get(std::size_t index) const noexcept;

  // Convenience accessors for dictionary children
  [[nodiscard]] std::optional<std::string_view> get_string(std::string_view key) const noexcept;
  [[nodiscard]] std::optional<int64_t> get_int(std::string_view key) const noexcept;
  [[nodiscard]] std::optional<std::span<const std::byte>> get_binary(std::string_view key) const noexcept;
  [[nodiscard]] std::string_view value_or(std::string_view key, std::string_view fallback) const noexcept;
  [[nodiscard]] int64_t value_or(std::string_view key, int64_t fallback) const noexcept;
  [[nodiscard]] std::span<const std::byte> binary_or(std::string_view key,
                                                     std::span<const std::byte> fallback = {}) const noexcept;

  // Dictionary indexing
  NodeView operator[](std::string_view key) const;

  // List indexing
  NodeView operator[](std::size_t index) const;
};

struct DictEntry {
  std::string_view first{};
  NodeView second{};

  constexpr DictEntry() noexcept = default;
  constexpr DictEntry(std::string_view k, NodeView v) noexcept : first(k), second(v) {}
};

inline constexpr NodeView::NodeView(std::span<const DictEntry> val) noexcept
    : nodeType(Type::Dictionary), spanValue{val.data(), val.size()} {}

inline std::optional<NodeView::DictType> NodeView::as_dict() const noexcept {
  if (nodeType == Type::Dictionary) {
    return DictType(static_cast<const DictEntry *>(spanValue.ptr), spanValue.len);
  }
  return std::nullopt;
}

inline std::optional<std::string_view> NodeView::get_string(std::string_view key) const noexcept {
  if (auto node = get(key)) {
    return node->as_string();
  }
  return std::nullopt;
}

inline std::optional<int64_t> NodeView::get_int(std::string_view key) const noexcept {
  if (auto node = get(key)) {
    return node->as_int();
  }
  return std::nullopt;
}

inline std::optional<std::span<const std::byte>> NodeView::get_binary(std::string_view key) const noexcept {
  if (auto node = get(key)) {
    return node->as_binary();
  }
  return std::nullopt;
}

inline std::string_view NodeView::value_or(std::string_view key, std::string_view fallback) const noexcept {
  return get_string(key).value_or(fallback);
}

inline int64_t NodeView::value_or(std::string_view key, int64_t fallback) const noexcept {
  return get_int(key).value_or(fallback);
}

inline std::span<const std::byte> NodeView::binary_or(std::string_view key,
                                                   std::span<const std::byte> fallback) const noexcept {
  return get_binary(key).value_or(fallback);
}

inline std::optional<NodeView> NodeView::get(std::string_view key) const noexcept {
  if (nodeType != Type::Dictionary) {
    return std::nullopt;
  }
  auto dict = *as_dict();
  auto it = std::lower_bound(
      dict.begin(), dict.end(), key,
      [](const DictEntry &entry, std::string_view k) {
        return entry.first < k;
      });
  if (it != dict.end() && it->first == key) {
    return it->second;
  }
  return std::nullopt;
}

inline std::optional<NodeView> NodeView::get(std::size_t index) const noexcept {
  if (nodeType == Type::List) {
    auto list = *as_list();
    if (index < list.size()) {
      return list[index];
    }
  }
  return std::nullopt;
}

inline NodeView NodeView::operator[](std::string_view key) const {
  if (auto val = get(key)) {
    return *val;
  }
#if BENCODE_ENABLE_EXCEPTIONS
  throw std::out_of_range("Key not found in Bencode dictionary: " + std::string(key));
#else
  return NodeView{};
#endif
}

inline NodeView NodeView::operator[](std::size_t index) const {
  if (auto val = get(index)) {
    return *val;
  }
#if BENCODE_ENABLE_EXCEPTIONS
  throw std::out_of_range("Index out of bounds in Bencode list: " + std::to_string(index));
#else
  return NodeView{};
#endif
}

} // namespace Bencode_Lib
