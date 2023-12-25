#pragma once

#include <string>

namespace Bencode_Lib {

struct String : Variant {
  String() : Variant(Variant::Type::string) {};
  explicit String(std::string string)
      : Variant(Variant::Type::string), bNodeString(std::move(string)) {}
  [[nodiscard]] std::string &value() { return (bNodeString); }
  [[nodiscard]] const std::string &value() const { return (bNodeString); }

private:
  std::string bNodeString;
};
} // namespace Bencode_Lib