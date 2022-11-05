#pragma once
//
// C++ STL
//
#include <type_traits>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ==============================
// BNode base reference converter
// ==============================
template <typename T> void CheckBNodeType(const Variant &bNodeVariant) {
  if constexpr (std::is_same_v<T, String>) {
    if (bNodeVariant.getNodeType() != Variant::Type::string) {
      throw BNode::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, Integer>) {
    if (bNodeVariant.getNodeType() != Variant::Type::integer) {
      throw BNode::Error("Node not an integer.");
    }
  } else if constexpr (std::is_same_v<T, List>) {
    if (bNodeVariant.getNodeType() != Variant::Type::list) {
      throw BNode::Error("Node not a list.");
    }
  } else if constexpr (std::is_same_v<T, Dictionary>) {
    if (bNodeVariant.getNodeType() != Variant::Type::dictionary) {
      throw BNode::Error("Node not a dictionary.");
    }
  }
}
template <typename T> T &BRef(BNode &bNode) {
  CheckBNodeType<T>(*bNode.getVariant());
  return (static_cast<T &>(*bNode.getVariant()));
}
template <typename T> const T &BRef(const BNode &bNode) {
  CheckBNodeType<T>(*bNode.getVariant());
  return (static_cast<const T &>(*bNode.getVariant()));
}
} // namespace Bencode_Lib