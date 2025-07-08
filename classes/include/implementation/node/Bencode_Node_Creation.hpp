#pragma once

namespace Bencode_Lib {
// Construct Node from raw values
template <typename T> Node::Node(T value) {
  if constexpr (std::is_integral_v<T>) {
    *this = make<Integer>(value);
  } else if constexpr (std::is_floating_point_v<T>) {
    *this = make<Integer>(static_cast<long long>(value));
  } else if constexpr (std::is_null_pointer_v<T>) {
    *this = make<Integer>(0);
  } else if constexpr (std::is_same_v<T, const char *> ||
                       std::is_same_v<T, std::string>) {
    *this = make<String>(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    bNodeVariant = std::move(value);
  }
}
// Convert the initializer list to Node
static Node typeToNode(const Bencode::InitializerListTypes &type) {
  if (const auto pValue = std::get_if<int>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<long>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<long long>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<float>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<double>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<long double>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<bool>(&type)) {
    return Node(*pValue);
  }
  if (const auto pValue = std::get_if<std::string>(&type)) {
    return Node(*pValue);
  }
  if ([[maybe_unused]] auto pValue = std::get_if<std::nullptr_t>(&type)) {
    return Node(0);
  }
  if (const auto pValue = std::get_if<Node>(&type)) {
    return std::move(*const_cast<Node *>(pValue));
  }
  throw Node::Error("Node of unsupported type could not be created.");
}
// Construct Node Array from the initializer list
inline Node::Node(const Bencode::ListInitializerType &list) {
  *this = make<List>();
  for (const auto &entry : list) {
    NRef<List>(*this).add(typeToNode(entry));
  }
}
// Construct Node Dictionary from the initializer list
inline Node::Node(const Bencode::DictionaryInitializerType &dictionary) {
  *this = make<Dictionary>();
  for (const auto &[fst, snd] : dictionary) {
    NRef<Dictionary>(*this).add(
        Dictionary::Entry(fst, typeToNode(snd)));
  }
}

} // namespace Bencode_Lib