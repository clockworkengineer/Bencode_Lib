#pragma once

namespace Bencode_Lib {

// Dictionary error
struct DictionaryError final : std::runtime_error {
  explicit DictionaryError(const std::string &message)
      : std::runtime_error("Dictionary Error: " + message) {}
};

// Dictionary entry
struct DictionaryEntry {
  DictionaryEntry(std::string key, BNode &&bNode)
      : key(BNode::make<String>(key)), bNode(std::move(bNode)) {}
  BNode &getKey() { return key; }
  [[nodiscard]] const std::string &getKey() const { return static_cast<const String &>(key.getVariant()).value(); }
  BNode &getBNode() { return bNode; }
  [[nodiscard]] const BNode &getBNode() const { return bNode; }

private:
  BNode key{};
  BNode bNode{};
};

// Dictionary variant
struct Dictionary : Variant {
  using Error = DictionaryError;
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
    std::ranges::sort(bNodeDictionary, []( const Entry &a, const Entry &b) {
      return a.getKey() < b.getKey();
    });
  }
  [[nodiscard]] bool contains(const std::string &key) const {
    try {
      auto _ = findEntryWithKey(bNodeDictionary, key);
    } catch (const Error &error) {
      return (false);
    }
    return (true);
  }
  [[nodiscard]] int size() const {
    return static_cast<int>(bNodeDictionary.size());
  }

  BNode &operator[](const std::string &key) {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }
  const BNode &operator[](const std::string &key) const {
    return findEntryWithKey(bNodeDictionary, key)->getBNode();
  }

  [[nodiscard]] Dictionary::Entries &value() { return bNodeDictionary; }
  [[nodiscard]] const Dictionary::Entries &value() const {
    return bNodeDictionary;
  }

private:
  template <typename T> static auto findEntry(T &dictionary, const std::string &key) {
    auto it = std::ranges::find_if(dictionary, [&key](const auto &entry) -> bool {
      return entry.getKey() == key;
    });
    if (it == dictionary.end()) {
      throw DictionaryError("Invalid key used in dictionary.");
    }
    return it;
  }

  static Dictionary::Entries::const_iterator
  findEntryWithKey(const Dictionary::Entries &dictionary,
                   const std::string &key) {
    return findEntry(dictionary, key);
  }

  static Dictionary::Entries::iterator
  findEntryWithKey(Dictionary::Entries &dictionary, const std::string &key) {
    return findEntry(dictionary, key);
  }

  Dictionary::Entries bNodeDictionary{};
};

} // namespace Bencode_Lib