#pragma once

namespace Bencode_Lib {

// Dictionary entry
struct DictionaryEntry {
  DictionaryEntry(const std::string_view key, BNode &&bNode)
      : key(BNode::make<String>(key)), bNode(std::move(bNode)) {}
  [[nodiscard]] std::string_view getKey() {
    return static_cast<String &>(key.getVariant()).value();
  }
  [[nodiscard]] std::string_view getKey() const {
    return static_cast<const String &>(key.getVariant()).value();
  }
  [[nodiscard]] BNode &getKeyBNode() { return key; }
  [[nodiscard]] const BNode &getKeyBNode() const { return key; }
  [[nodiscard]] BNode &getBNode() { return bNode; }
  [[nodiscard]] const BNode &getBNode() const { return bNode; }

private:
  BNode key{};
  BNode bNode{};
};

// Dictionary variant
struct Dictionary : Variant {
  using Entry = DictionaryEntry;
  using Entries = std::vector<Entry>;
  // Constructors/Destructors
  Dictionary() : Variant(Type::dictionary) {}
  Dictionary(const Dictionary &other) = default;
  Dictionary &operator=(const Dictionary &other) = default;
  Dictionary(Dictionary &&other) = default;
  Dictionary &operator=(Dictionary &&other) = default;
  ~Dictionary() = default;
  // Add Entry to Dictionary
  template <typename T> void add(T &&entry) {
    bNodeDictionary.emplace_back(std::forward<T>(entry));
    std::ranges::sort(bNodeDictionary, [](const Entry &a, const Entry &b) {
      return a.getKey() < b.getKey();
    });
  }
  [[nodiscard]] bool contains( std::string_view key) const {
    try {
      findEntryWithKey(bNodeDictionary, key);
    } catch ([[maybe_unused]] const BNode::Error &error) {
      return false;
    }
    return true;
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }

  BNode &operator[]( std::string_view key) {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }
  const BNode &operator[]( std::string_view key) const {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }

  [[nodiscard]] Entries &value() { return bNodeDictionary; }
  [[nodiscard]] const Entries &value() const {
    return bNodeDictionary;
  }

private:
  template <typename T>
  static auto findEntry(T &dictionary, const std::string_view &key) {
    auto it =
        std::ranges::find_if(dictionary, [&key](const auto &entry) -> bool {
          return entry.getKey() == key;
        });
    if (it == dictionary.end()) {
      throw BNode::Error("Invalid key used in dictionary.");
    }
    return it;
  }

  static Entries::const_iterator
  findEntryWithKey(const Entries &dictionary,
                    const std::string_view &key) {
    return findEntry(dictionary, key);
  }

  static Entries::iterator
  findEntryWithKey(Entries &dictionary, std::string_view key) {
    return findEntry(dictionary, key);
  }

  Entries bNodeDictionary{};
};

} // namespace Bencode_Lib