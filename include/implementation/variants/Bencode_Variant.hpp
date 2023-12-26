#pragma once

namespace Bencode_Lib {

struct Variant {
  enum class Type { base = 0, dictionary, list, integer, string };
  // Constructors/Destructors
  explicit Variant(Type nodeType = Type::base) : nodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Get BNode type
  [[nodiscard]] Type getNodeType() const { return (nodeType); }

private:
  Variant::Type nodeType;
};
} // namespace Bencode_Lib