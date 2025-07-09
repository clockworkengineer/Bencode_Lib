#pragma once

namespace Bencode_Lib {

// ============================
// Bencode forward declarations
// ============================
class IStringify;
class IParser;
class ISource;
class IDestination;
class IAction;
class Bencode_Impl;
struct Node;

class Bencode {

public:
  // Bencode maximum integer type
  using IntegerType = int64_t;
  // Bencode maximum string type
  using StringType = std::string;
  // Bencode maximum list type
  using ListType = std::vector<Node>;
  // Bencode maximum dictionary type
  using DictionaryType = std::map<std::string, Node>;
  // Possible Bencode Node initializer types
  using InitializerListTypes =
      std::variant<int, long, long long, float, double, long double, bool,
                   std::string, std::nullptr_t, Node>;
  // List initializer list
  using ListInitializerType = std::initializer_list<InitializerListTypes>;
  // Dictionary initializer list
  using DictionaryInitializerType =
      std::initializer_list<std::pair<std::string, InitializerListTypes>>;
  // Constructors/Destructors
  explicit Bencode([[maybe_unused]] IStringify *stringify = nullptr,
                   [[maybe_unused]] IParser *parser = nullptr);
  // Pass in default JSON to parse
  explicit Bencode(const std::string_view &bencodeString);
  // Construct an array
  Bencode(const ListInitializerType &list);
  // Construct object
  Bencode(const DictionaryInitializerType &dictionary);
  Bencode(const Bencode &other) = delete;
  Bencode &operator=(const Bencode &other) = delete;
  Bencode(Bencode &&other) = delete;
  Bencode &operator=(Bencode &&other) = delete;
  ~Bencode();
  // Parse Bencode into Node tree
  void parse(ISource &source) const;
  void parse(ISource &&source) const;
  // Stringify Bencode from Node tree
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;
  // Return Bencode_Lib version
  [[nodiscard]] static std::string version();
  // Return Node tree root
  [[nodiscard]] Node &root();
  [[nodiscard]] const Node &root() const;
  // Traverse Bencode tree
  [[maybe_unused]] void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Search for Bencode dictionary entry with a given key
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  // Get Bencode list entry at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Read/Write Bencode from the file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &bencodeString);

private:
  const std::unique_ptr<Bencode_Impl> implementation;
};

} // namespace Bencode_Lib
