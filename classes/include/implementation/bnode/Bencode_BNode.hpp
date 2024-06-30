#pragma once

namespace Bencode_Lib {

struct BNode {
  // BNode Error
  struct Error final : std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("BNode Error: " + message) {}
  };
  // Constructors/Destructors
  BNode() = default;
  template <typename T> explicit BNode(T value);
  BNode(const Bencode::ListInitializer &list);
  BNode(const Bencode::DictionaryInitializer &dictionary);
  BNode(const BNode &other) = delete;
  BNode &operator=(const BNode &other) = delete;
  BNode(BNode &&other) = default;
  BNode &operator=(BNode &&other) = default;
  ~BNode() = default;
  // Assignment operators
  template <typename T> BNode &operator=(T value) {
    return *this = BNode(value);
  }
  // Indexing operators
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](int index);
  const BNode &operator[](int index) const;
  // Interrogate variant
  [[nodiscard]] bool isEmpty() const { return bNodeVariant == nullptr; }
  // Get reference to BNode variant
  [[nodiscard]] Variant &getVariant() {
    return *bNodeVariant;
  }
  [[nodiscard]] const Variant &getVariant() const {
    return *bNodeVariant;
  }
  // Make BNode
  template <typename T, typename... Args> static auto make(Args &&...args) {
    return BNode{std::make_unique<T>(std::forward<Args>(args)...)};
  }

private:
  static BNode typeToBNode(const Bencode::intializerListTypes &type);
  std::unique_ptr<Variant> bNodeVariant;
};

} // namespace Bencode_Lib