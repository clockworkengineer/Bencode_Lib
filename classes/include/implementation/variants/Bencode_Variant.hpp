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
  // Determine BNode type
  [[nodiscard]] bool isString() const {
    return nodeType == Type::string;
  }
  [[nodiscard]] bool isInteger() const {
    return nodeType == Type::integer;
  }
  [[nodiscard]] bool isList() const {
    return nodeType == Type::list;
  }
  [[nodiscard]] bool isDictionary() const {
    return nodeType == Type::dictionary;
  }
  [[nodiscard]] bool isHole() const {
    return nodeType == Type::hole;
  }
private:
  Type nodeType;
};
} // namespace Bencode_Lib