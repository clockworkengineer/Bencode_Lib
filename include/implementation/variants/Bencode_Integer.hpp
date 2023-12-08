#pragma once

namespace Bencode_Lib {

struct Integer : Variant {
  explicit Integer(int64_t integer)
      : Variant(Variant::Type::integer), bNodeInteger(integer) {}
  [[nodiscard]] int64_t value() const { return (bNodeInteger); }

private:
  int64_t bNodeInteger = 0;
};
} // namespace Bencode_Lib