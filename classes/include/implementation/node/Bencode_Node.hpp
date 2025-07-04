#pragma once

namespace Bencode_Lib {

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
  template <typename T> Node &operator=(T value) {
    return *this = Node(value);
  }
  // Indexing operators
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  Node &operator[](int index);
  const Node &operator[](int index) const;
  // Interrogate variant
  [[nodiscard]] bool isEmpty() const { return bNodeVariant == nullptr; }
  // Get reference to Node variant
  [[nodiscard]] Variant &getVariant() {
    return *bNodeVariant;
  }
  [[nodiscard]] const Variant &getVariant() const {
    return *bNodeVariant;
  }
  // Make Node
  template <typename T, typename... Args> static auto make(Args &&...args) {
    return Node{std::make_unique<T>(std::forward<Args>(args)...)};
  }

private:
  std::unique_ptr<Variant> bNodeVariant;
};

} // namespace Bencode_Lib