#pragma once

namespace Bencode_Lib {

struct Integer : Variant {
  // Constructors/Destructors
  Integer() : Variant(Variant::Type::integer) {}
  explicit Integer(const int64_t integer)
      : Variant(Variant::Type::integer), bNodeInteger(integer) {}
  Integer(const Integer &other) = default;
  Integer &operator=(const Integer &other) = default;
  Integer(Integer &&other) = default;
  Integer &operator=(Integer &&other) = default;
  ~Integer() = default;
  // Get BNode value
  [[nodiscard]] int64_t value() const { return bNodeInteger; }

private:
  int64_t bNodeInteger = 0;
};
} // namespace Bencode_Lib