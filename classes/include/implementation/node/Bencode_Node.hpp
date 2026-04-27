#pragma once

#include <atomic>
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

template <typename T> class ObjectPool {
public:
  struct FreeNode {
    FreeNode *next = nullptr;
  };

  struct Stats {
    size_t acquired = 0;
    size_t created = 0;
    size_t reused = 0;
    size_t released = 0;
  };

  static T *acquire() noexcept {
    ++totalAcquired;
    if (freeList) {
      ++totalReused;
      FreeNode *node = freeList;
      freeList = node->next;
      return reinterpret_cast<T *>(node);
    }
    ++totalCreated;
    return static_cast<T *>(::operator new(sizeof(T)));
  }

  static void release(T *value) noexcept {
    if (!value) {
      return;
    }
    ++totalReleased;
    value->~T();
    FreeNode *node = reinterpret_cast<FreeNode *>(value);
    node->next = freeList;
    freeList = node;
  }

  static Stats getStats() noexcept {
    return Stats{totalAcquired, totalCreated, totalReused, totalReleased};
  }

  static void resetStats() noexcept {
    totalAcquired = 0;
    totalCreated = 0;
    totalReused = 0;
    totalReleased = 0;
  }

private:
  inline static std::atomic<size_t> totalAcquired = 0;
  inline static std::atomic<size_t> totalCreated = 0;
  inline static std::atomic<size_t> totalReused = 0;
  inline static std::atomic<size_t> totalReleased = 0;
  static thread_local FreeNode *freeList;
};

template <typename T>
thread_local typename ObjectPool<T>::FreeNode *ObjectPool<T>::freeList =
    nullptr;

template <typename T> struct PoolDeleter {
  void operator()(T *value) const noexcept { ObjectPool<T>::release(value); }
};

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
      Variant &
      operator()(const std::unique_ptr<List, PoolDeleter<List>> &value) const {
        return dereferenceList(value);
      }
      Variant &operator()(
          const std::unique_ptr<Dictionary, PoolDeleter<Dictionary>> &value)
          const {
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
      const Variant &
      operator()(const std::unique_ptr<List, PoolDeleter<List>> &value) const {
        return dereferenceList(value);
      }
      const Variant &operator()(
          const std::unique_ptr<Dictionary, PoolDeleter<Dictionary>> &value)
          const {
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
      T *raw = ObjectPool<T>::acquire();
      new (raw) T(std::forward<Args>(args)...);
      if constexpr (std::is_same_v<T, List>) {
        bNodeVariant = std::unique_ptr<List, PoolDeleter<List>>(raw);
      } else {
        bNodeVariant =
            std::unique_ptr<Dictionary, PoolDeleter<Dictionary>>(raw);
      }
    } else {
      bNodeVariant = T(std::forward<Args>(args)...);
    }
  }

  static Variant &
  dereferenceList(const std::unique_ptr<List, PoolDeleter<List>> &value);
  static Variant &dereferenceDictionary(
      const std::unique_ptr<Dictionary, PoolDeleter<Dictionary>> &value);

  using Storage =
      std::variant<std::monostate, Hole, Integer, String,
                   std::unique_ptr<List, PoolDeleter<List>>,
                   std::unique_ptr<Dictionary, PoolDeleter<Dictionary>>>;
  Storage bNodeVariant{};
};

} // namespace Bencode_Lib

#include "Bencode_List.hpp"
#include "Bencode_Dictionary.hpp"

namespace Bencode_Lib {

template <typename T, typename... Args> Node Node::make(Args &&...args) {
  return Node(std::in_place_type<T>, std::forward<Args>(args)...);
}

inline Variant &
Node::dereferenceList(const std::unique_ptr<List, PoolDeleter<List>> &value) {
  return *value;
}
inline Variant &Node::dereferenceDictionary(
    const std::unique_ptr<Dictionary, PoolDeleter<Dictionary>> &value) {
  return *value;
}

} // namespace Bencode_Lib