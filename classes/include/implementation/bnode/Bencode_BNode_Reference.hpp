#pragma once

#include <type_traits>

namespace Bencode_Lib {

// ==============================
// BNode base reference converter
// ==============================
template <typename T> void CheckBNodeType(const BNode &bNodeVariant) {
  if constexpr (std::is_same_v<T, String>) {
    if (!bNodeVariant.isString()) {
      throw BNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, Integer>) {
    if (!bNodeVariant.isInteger()) {
      throw BNode::Error("Node not an integer.");
    }
  } else if constexpr (std::is_same_v<T, List>) {
    if (!bNodeVariant.isList()) {
      throw BNode::Error("Node not a list.");
    }
  } else if constexpr (std::is_same_v<T, Dictionary>) {
    if (!bNodeVariant.isDictionary()) {
      throw BNode::Error("Node not a dictionary.");
    }
  }
}
template <typename T> T &BRef(BNode &bNode) {
  CheckBNodeType<T>(bNode);
  return static_cast<T &>(bNode.getVariant());
}
template <typename T> const T &BRef(const BNode &bNode) {
  CheckBNodeType<T>(bNode);
  return static_cast<const T &>(bNode.getVariant());
}

} // namespace Bencode_Lib