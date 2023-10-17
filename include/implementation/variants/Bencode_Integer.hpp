#pragma once

namespace Bencode_Lib {

struct Integer : Variant {
  explicit Integer(int64_t integer)
      : Variant(Variant::Type::integer), value(integer) {}
  [[nodiscard]] int64_t integer() const { return (value); }

private:
  int64_t value = 0;
};
} // namespace Bencode_Lib