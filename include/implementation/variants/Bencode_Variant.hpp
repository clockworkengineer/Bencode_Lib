#pragma once

namespace Bencode_Lib {

struct Variant {
  enum class Type { base = 0, dictionary, list, integer, string, hole };
  // Constructors/Destructors
  explicit Variant(Type nodeType = Type::base) : nodeType(nodeType) {}
  Variant(const Variant &other) = default;
  Variant &operator=(const Variant &other) = default;
  Variant(Variant &&other) = default;
  Variant &operator=(Variant &&other) = default;
  ~Variant() = default;
  // Determine BNode type
  [[nodiscard]] bool isString() const {
    return (nodeType == Variant::Type::string);
  }
  [[nodiscard]] bool isInteger() const {
    return (nodeType == Variant::Type::integer);
  }
  [[nodiscard]] bool isList() const {
    return (nodeType == Variant::Type::list);
  }
  [[nodiscard]] bool isDictionary() const {
    return (nodeType == Variant::Type::dictionary);
  }
  [[nodiscard]] bool isHole() const {
    return (nodeType == Variant::Type::hole);
  }
private:
  Variant::Type nodeType;
};
} // namespace Bencode_Lib