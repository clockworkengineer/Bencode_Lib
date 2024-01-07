#pragma once

#include <string>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <memory>

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Impl {

public:
  Bencode_Impl();
  Bencode_Impl(const Bencode_Impl &other) = delete;
  Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
  Bencode_Impl(Bencode_Impl &&other) = delete;
  Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
  ~Bencode_Impl();

  void setEncoder(IEncoder *encoder);
  void setDecoder(IDecoder *decoder);
  void decode(ISource &source);
  void encode(IDestination &destination) const;
  std::string version() const;
  [[nodiscard]] BNode &root() { return (bNodeRoot); }
  [[nodiscard]] const BNode &root() const { return (bNodeRoot); }
  void traverse(IAction &action);
  void traverse(IAction &action) const;
  BNode &operator[](const std::string &key);
  const BNode &operator[](const std::string &key) const;
  BNode &operator[](std::size_t index);
  const BNode &operator[](std::size_t index) const;

private:
  // Traverse Bencode BNode tree
  template <typename T> static void traverseBNodes(T &bNode, IAction &action);
  // Root of BNode tree
  BNode bNodeRoot;
  // Bencode encoder
  std::unique_ptr<IEncoder> bNodeEncoder{};
  // Bencode decoder
  std::unique_ptr<IDecoder> bNodeDecoder{};
};

/// <summary>
/// Recursively traverse BNode tree calling IAction methods and possibly
/// modifying the tree contents or even structure.
/// </summary>
/// <param name=bNode>BNode tree to be traversed.</param>
/// <param name=action>Action methods to call during traversal.</param>
template <typename T>
void Bencode_Impl::traverseBNodes(T &bNode, IAction &action) {
  action.onBNode(bNode);
  if (bNode.isInteger()) {
    action.onInteger(bNode);
  } else if (bNode.isString()) {
    action.onString(bNode);
  } else if (bNode.isDictionary()) {
    action.onDictionary(bNode);
    for (auto &entry : BRef<Dictionary>(bNode).value()) {
      traverseBNodes(entry.getBNode(), action);
    }
  } else if (bNode.isList()) {
    action.onList(bNode);
    for (auto &entry : BRef<List>(bNode).value()) {
      traverseBNodes(entry, action);
    }
  } else if (!bNode.isHole()) {
    throw Bencode::Error(
        "Unknown BNode type encountered during tree traversal.");
  }
}

} // namespace Bencode_Lib
