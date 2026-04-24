#pragma once

#include <string>
#include <string_view>

namespace Bencode_Lib {

struct String : Variant {
#if defined(BENCODE_MAX_STRING_LENGTH)
  constexpr static Bencode::IntegerType kMaxLength = BENCODE_MAX_STRING_LENGTH;
#else
  constexpr static Bencode::IntegerType kMaxLength = 16 * 1024;
#endif
  // Constructors/Destructors
  String() : Variant(Type::string), bNodeString("") {}
  explicit String(const std::string_view string)
      : Variant(Type::string), bNodeString(string) {}
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Get Node value
  [[nodiscard]] std::string_view value() { return bNodeString; }
  [[nodiscard]] std::string_view value() const { return bNodeString; }
  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) {
    maxStringLength = length;
  }
  static uint64_t getMaxStringLength() { return maxStringLength; }

private:
  std::string bNodeString;
  inline static uint64_t maxStringLength{kMaxLength};
};
} // namespace Bencode_Lib