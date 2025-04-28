#pragma once

namespace Bencode_Lib {

struct Integer : Variant {
  // Constructors/Destructors
  Integer() : Variant(Type::integer) {}
  explicit Integer(const Bencode::IntegerType integer)
      : Variant(Type::integer), bNodeInteger(integer) {}
  Integer(const Integer &other) = default;
  Integer &operator=(const Integer &other) = default;
  Integer(Integer &&other) = default;
  Integer &operator=(Integer &&other) = default;
  ~Integer() = default;
  // Get BNode value
  [[nodiscard]] Bencode::IntegerType value() const { return bNodeInteger; }

private:
  Bencode::IntegerType bNodeInteger = 0;
};
} // namespace Bencode_Lib