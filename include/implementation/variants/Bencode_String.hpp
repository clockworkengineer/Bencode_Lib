#pragma once
//
// C++ STL
//
#include <string>
// =================
// LIBRARY NAMESPACE
// =================
namespace Bencode_Lib {
// ======
// String
// ======
struct String : Variant {
  explicit String(std::string string)
      : Variant(Variant::Type::string), m_string(std::move(string)) {}
  [[nodiscard]] std::string &string() { return (m_string); }
  [[nodiscard]] const std::string &string() const { return (m_string); }

private:
  std::string m_string;
};
} // namespace Bencode_Lib