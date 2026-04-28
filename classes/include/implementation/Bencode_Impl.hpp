// File: Bencode_Impl.hpp
//
// Description: Private Bencode implementation class that manages parser, stringifier, and root node state.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include <type_traits>
#include <utility>

namespace Bencode_Lib {

class Bencode_Impl {

public:
  // Constructors/Destructors
  Bencode_Impl(IStringify *stringify, IParser *parser);
  Bencode_Impl(const Bencode_Impl &other) = delete;
  Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
  Bencode_Impl(Bencode_Impl &&other) = delete;
  Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
  ~Bencode_Impl();

  using ParseResultType = std::conditional_t<BENCODE_ENABLE_EXCEPTIONS, void, ParseStatus>;

  // Parse Bencoded source into Node tree
  ParseResultType parse(ISource &source);
  ParseResultType parse(ISource &&source);

  template <typename Source>
  ParseResultType parseSource(Source &&source) {
    return parse(std::forward<Source>(source));
  }

  // Stringify Node tree
  void stringify(IDestination &destination) const;
  void stringify(IDestination &&destination) const;

  template <typename Destination>
  void stringifyDestination(Destination &&destination) const {
    stringify(std::forward<Destination>(destination));
  }

  // Bencode version
  static std::string version();
  // Return root Node of the tree
  [[nodiscard]] Node &root() { return bNodeRoot; }
  [[nodiscard]] const Node &root() const { return bNodeRoot; }
  // Traverse Node tree
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  // Search for Bencode dictionary entry with a given key
  Node &operator[](const std::string_view &key);
  const Node &operator[](const std::string_view &key) const;
  // Ensure the root is a dictionary or list, creating it if empty.
  Node &ensureDictionaryRoot();
  Node &ensureListRoot();
  // Get Bencode list element at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Read/Write Bencode from a file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName,
                     const std::string_view &bencodeString);

private:
  ParseResultType parseImpl(ISource &source);
  void traverseImpl(IAction &action) const;
  void ensureNotEmpty() const;
  Node &getOrCreateDictionaryEntry(const std::string_view &key);
  Node &getOrCreateListEntry(std::size_t index);
#if BENCODE_ENABLE_EXCEPTIONS
  ParseResultType handleParseResult(ISource &source);
#else
  ParseResultType handleParseResult(ParseStatus result, ISource &source);
#endif

  // Traverse Bencode Node tree
  template <typename T> static void traverseNodes(T &bNode, IAction &action);
  // Root of Node tree
  Node bNodeRoot;
  // Bencode stringify
  std::unique_ptr<IStringify> bNodeStringify{};
  // Bencode parser
  std::unique_ptr<IParser> bNodeParser{};
};

/// <summary>
/// Recursively traverse Node tree calling IAction methods and possibly
/// modifying the tree contents or even structure.
/// </summary>
/// <param name="bNode">Node tree to be traversed.</param>
/// <param name="action">Action methods to call during traversal.</param>
template <typename T>
void Bencode_Impl::traverseNodes(T &bNode, IAction &action) {
  action.onNode(bNode);
  if (isA<Integer>(bNode)) {
    action.onInteger(bNode);
  } else if (isA<String>(bNode)) {
    action.onString(bNode);
  } else if (isA<Dictionary>(bNode)) {
    action.onDictionary(bNode);
    for (auto &entry : NRef<Dictionary>(bNode).value()) {
      traverseNodes(entry.getNode(), action);
    }
  } else if (isA<List>(bNode)) {
    action.onList(bNode);
    for (auto &entry : NRef<List>(bNode).value()) {
      traverseNodes(entry, action);
    }
  } else if (isA<Hole>(bNode)) {
    throw Error("Unknown Node type encountered during tree traversal.");
  }
}

} // namespace Bencode_Lib
