#pragma once

namespace Bencode_Lib {

struct Variant {
  enum class Type { base = 0, dictionary, list, integer, string };
  explicit Variant(Type nodeType = Type::base) : nodeType(nodeType) {}
  Variant(const Variant &other) = delete;
  Variant &operator=(const Variant &other) = delete;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  [[nodiscard]] Type getNodeType() const { return (nodeType); }

private:
  Variant::Type nodeType;
};
} // namespace Bencode_Lib