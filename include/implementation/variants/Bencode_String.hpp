#pragma once

#include <string>

namespace Bencode_Lib {

struct String : Variant {
  explicit String(std::string string)
      : Variant(Variant::Type::string), bNodeString(std::move(string)) {}
  [[nodiscard]] std::string &string() { return (bNodeString); }
  [[nodiscard]] const std::string &string() const { return (bNodeString); }

private:
  std::string bNodeString;
};
} // namespace Bencode_Lib