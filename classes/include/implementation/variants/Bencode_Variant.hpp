#pragma once

namespace Bencode_Lib {

struct Variant {
  enum class Type : std::uint8_t { base = 0, dictionary, list, integer, string, hole };
  // Constructors/Destructors
  explicit Variant(const Type nodeType = Type::base) : nodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Get BNode type
  [[nodiscard]] Type getNodeType() const {
    return nodeType;
  }
private:
  Type nodeType;
};
} // namespace Bencode_Lib