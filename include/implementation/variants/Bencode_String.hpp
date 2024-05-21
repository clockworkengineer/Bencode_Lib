#pragma once

#include <string>

namespace Bencode_Lib {

struct String : Variant {
  // Constructors/Destructors
  String() : Variant(Type::string){}
  explicit String(const std::string &string)
      : Variant(Type::string), bNodeString(string) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Get BNode value
  [[nodiscard]] std::string &value() { return bNodeString; }
  [[nodiscard]] const std::string &value() const { return bNodeString; }

private:
  std::string bNodeString;
};
} // namespace Bencode_Lib