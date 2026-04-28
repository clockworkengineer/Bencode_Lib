// File: Bencode_String.hpp
//
// Description: String variant implementation representing raw byte string values in Bencode.
//

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace Bencode_Lib {

struct String : Variant {
#if defined(BENCODE_MAX_STRING_LENGTH)
  constexpr static std::uint64_t kMaxLength = BENCODE_MAX_STRING_LENGTH;
#else
  constexpr static std::uint64_t kMaxLength = 16 * 1024;
#endif

  // Constructors/Destructors
  String() : Variant(Type::string), mValue() {}
  explicit String(const std::string_view string)
      : Variant(Type::string), mValue(string.data(), string.size()) {}
  explicit String(const char *string) : Variant(Type::string), mValue(string) {}
  explicit String(std::string &&string)
      : Variant(Type::string), mValue(std::move(string)) {}
  explicit String(std::size_t length)
      : Variant(Type::string), mValue(length, '\0') {}

  String(const String &other) = default;
  String(String &&other) noexcept = default;
  String &operator=(const String &other) = default;
  String &operator=(String &&other) noexcept = default;
  ~String() = default;

  // Get Node value
  [[nodiscard]] std::string_view value() const noexcept {
    return std::string_view(mValue.data(), mValue.size());
  }

  [[nodiscard]] char *data() noexcept { return mValue.data(); }
  [[nodiscard]] const char *data() const noexcept { return mValue.data(); }

  void resize(std::size_t newSize) { mValue.resize(newSize); }

  void assign(const std::string_view string) {
    mValue.assign(string.data(), string.size());
  }

  void assign(const char *data, const std::size_t length) {
    mValue.assign(data, length);
  }

  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) {
    maxStringLength = length;
  }
  static uint64_t getMaxStringLength() { return maxStringLength; }

private:
  std::string mValue;
  inline static uint64_t maxStringLength{kMaxLength};
};

} // namespace Bencode_Lib