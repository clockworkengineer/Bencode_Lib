#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#include "Bencode_Variant.hpp"
#include "Bencode_Hole.hpp"
#include "Bencode_Integer.hpp"
#include "Bencode_String.hpp"

namespace Bencode_Lib {
class Bencode;
struct List;
struct Dictionary;

struct Node {
  // Node Error
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view &message)
        : std::runtime_error(std::string("Node Error: ").append(message)) {}
  };
  // Constructors/Destructors
  Node() = default;
  template <typename T> explicit Node(T value);
  Node(const Bencode::ListInitializerType &list);
  Node(const Bencode::DictionaryInitializerType &dictionary);
  Node(const Node &other) = delete;
  Node &operator=(const Node &other) = delete;
  Node(Node &&other) = default;
  Node &operator=(Node &&other) = default;
  ~Node() = default;
  // Assignment operators
  template <typename T> Node &operator=(T value) { return *this = Node(value); }
  Node &operator=(const Bencode::ListInitializerType &list);
  Node &operator=(const Bencode::DictionaryInitializerType &dictionary);
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](int index);
  const Node &operator[](int index) const;
  // Interrogate variant
  [[nodiscard]] bool isEmpty() const {
    return std::holds_alternative<std::monostate>(bNodeVariant);
  }
  // Get reference to Node variant
  [[nodiscard]] Variant &getVariant() {
    struct Visitor {
      Variant &operator()(Hole &value) const noexcept { return value; }
      Variant &operator()(Integer &value) const noexcept { return value; }
      Variant &operator()(String &value) const noexcept { return value; }
      Variant &operator()(const std::unique_ptr<List> &value) const {
        return dereferenceList(value);
      }
      Variant &operator()(const std::unique_ptr<Dictionary> &value) const {
        return dereferenceDictionary(value);
      }
      Variant &operator()(std::monostate &) const {
        throw Error("Node is empty.");
      }
    };
    return std::visit(Visitor{}, bNodeVariant);
  }
  [[nodiscard]] const Variant &getVariant() const {
    struct Visitor {
      const Variant &operator()(const Hole &value) const noexcept {
        return value;
      }
      const Variant &operator()(const Integer &value) const noexcept {
        return value;
      }
      const Variant &operator()(const String &value) const noexcept {
        return value;
      }
      const Variant &operator()(const std::unique_ptr<List> &value) const {
        return dereferenceList(value);
      }
      const Variant &
      operator()(const std::unique_ptr<Dictionary> &value) const {
        return dereferenceDictionary(value);
      }
      const Variant &operator()(const std::monostate &) const {
        throw Error("Node is empty.");
      }
    };
    return std::visit(Visitor{}, bNodeVariant);
  }
  // Make Node
  template <typename T, typename... Args> static Node make(Args &&...args);

private:
  template <typename T, typename... Args>
  explicit Node(std::in_place_type_t<T>, Args &&...args) {
    if constexpr (std::is_same_v<T, List> || std::is_same_v<T, Dictionary>) {
      bNodeVariant = std::make_unique<T>(std::forward<Args>(args)...);
    } else {
      bNodeVariant = T(std::forward<Args>(args)...);
    }
  }

  static Variant &dereferenceList(const std::unique_ptr<List> &value);
  static Variant &
  dereferenceDictionary(const std::unique_ptr<Dictionary> &value);

  using Storage =
      std::variant<std::monostate, Hole, Integer, String, std::unique_ptr<List>,
                   std::unique_ptr<Dictionary>>;
  Storage bNodeVariant{};
};

} // namespace Bencode_Lib

#include "Bencode_List.hpp"
#include "Bencode_Dictionary.hpp"

namespace Bencode_Lib {

inline Variant &Node::dereferenceList(const std::unique_ptr<List> &value) {
  return *value;
}
inline Variant &
Node::dereferenceDictionary(const std::unique_ptr<Dictionary> &value) {
  return *value;
}

template <typename T, typename... Args> Node Node::make(Args &&...args) {
  if constexpr (std::is_same_v<T, List> || std::is_same_v<T, Dictionary>) {
    return Node(std::make_unique<T>(std::forward<Args>(args)...));
  } else {
    return Node(std::in_place_type<T>, std::forward<Args>(args)...);
  }
}

} // namespace Bencode_Lib