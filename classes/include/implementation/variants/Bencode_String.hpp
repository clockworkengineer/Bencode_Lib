#pragma once

namespace Bencode_Lib {

struct BencodeString {
  explicit BencodeString(const std::string_view &string) {
    bencodeString.reset(static_cast<char *>(malloc(string.size())));
    BencodeStringLength = string.size();
    std::memcpy(&bencodeString[0], string.data(), BencodeStringLength);
  }

  [[nodiscard]] std::string_view value() const {
    return { bencodeString.get(), BencodeStringLength};
  }
private:
  std::unique_ptr<char[]> bencodeString;
  std::size_t  BencodeStringLength {};
};

struct String : Variant {
  constexpr static Bencode::IntegerType kMaxLength = 16*1024;
  // Constructors/Destructors
  String() : Variant(Type::string) , bNodeString(""){}
  explicit String(const std::string_view string)
      : Variant(Type::string), bNodeString(string) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Get BNode value
  [[nodiscard]] std::string_view value() { return bNodeString.value(); }
  [[nodiscard]] std::string_view value() const { return bNodeString.value(); }
  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) { maxStringLength = length; }
  static uint64_t getMaxStringLength() { return maxStringLength; }
private:
  BencodeString bNodeString;
  inline static uint64_t maxStringLength { kMaxLength};
};
} // namespace Bencode_Lib