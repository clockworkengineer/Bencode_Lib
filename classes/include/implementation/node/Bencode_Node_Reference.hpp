#pragma once

namespace Bencode_Lib {
// What is Node variant?
template <typename T> bool isA(const Node &bNode) {
  if constexpr (std::is_same_v<T, String>) {
    return bNode.getVariant().getNodeType() == Variant::Type::string;
  } else if constexpr (std::is_same_v<T, Integer>) {
    return bNode.getVariant().getNodeType() == Variant::Type::integer;
  } else if constexpr (std::is_same_v<T, List>) {
    return bNode.getVariant().getNodeType() == Variant::Type::list;
  } else if constexpr (std::is_same_v<T, Dictionary>) {
    return bNode.getVariant().getNodeType() == Variant::Type::dictionary;
  } else if constexpr (std::is_same_v<T, Hole>) {
    return bNode.getVariant().getNodeType() == Variant::Type::hole;
  } else {
    return false;
  }
}
// Node base reference converter
template <typename T> void CheckNodeType(const Node &bNode) {
  if constexpr (std::is_same_v<T, String>) {
    if (!isA<T>(bNode)) {
      throw Node::Error("Node not a string.");
    }
  } else if constexpr (std::is_same_v<T, Integer>) {
    if (!isA<T>(bNode)) {
      throw Node::Error("Node not an integer.");
    }
  } else if constexpr (std::is_same_v<T, List>) {
    if (!isA<T>(bNode)) {
      throw Node::Error("Node not a list.");
    }
  } else if constexpr (std::is_same_v<T, Dictionary>) {
    if (!isA<T>(bNode)) {
      throw Node::Error("Node not a dictionary.");
    }
  } else if constexpr (std::is_same_v<T, Hole>) {
    if (!isA<T>(bNode)) {
      throw Node::Error("Node not a hole.");
    }
  }
}
template <typename T> T &NRef(Node &bNode) {
  CheckNodeType<T>(bNode);
  return static_cast<T &>(bNode.getVariant());
}
template <typename T> const T &NRef(const Node &bNode) {
  CheckNodeType<T>(bNode);
  return static_cast<const T &>(bNode.getVariant());
}

} // namespace Bencode_Lib