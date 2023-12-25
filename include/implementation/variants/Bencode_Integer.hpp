#pragma once

namespace Bencode_Lib {

struct Integer : Variant {
  // Constructors/Destructors
  Integer() : Variant(Variant::Type::integer) {}
  explicit Integer(int64_t integer)
      : Variant(Variant::Type::integer), bNodeInteger(integer) {}
  // Get BNode value
  [[nodiscard]] int64_t value() const { return (bNodeInteger); }

private:
  int64_t bNodeInteger = 0;
};
} // namespace Bencode_Lib