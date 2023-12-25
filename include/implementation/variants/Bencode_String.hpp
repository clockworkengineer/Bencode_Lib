#pragma once

#include <string>

namespace Bencode_Lib {

struct String : Variant {
  // Constructors/Destructors
  String() : Variant(Variant::Type::string) {};
  explicit String(std::string string)
      : Variant(Variant::Type::string), bNodeString(std::move(string)) {}
  // Get BNode value
  [[nodiscard]] std::string &value() { return (bNodeString); }
  [[nodiscard]] const std::string &value() const { return (bNodeString); }

private:
  std::string bNodeString;
};
} // namespace Bencode_Lib