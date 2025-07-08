#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

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
  // Parse Bencoded source into Node tree
  void parse(ISource &source);
  // Stringify Node tree
  void stringify(IDestination &destination) const;
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
  // Get Bencode list element at index
  Node &operator[](std::size_t index);
  const Node &operator[](std::size_t index) const;
  // Read/Write Bencode from a file
  static std::string fromFile(const std::string_view &fileName);
  static void toFile(const std::string_view &fileName, const std::string_view &bencodeString);

private:
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
