#pragma once

namespace Bencode_Lib {

struct BencodeString {
  explicit BencodeString(const std::string &str) {
    bencodeString.reset(static_cast<char *>(malloc(str.size())));
    BencodeStringLength = str.size();
    std::memcpy(&bencodeString[0], str.c_str(), BencodeStringLength);
  }

  [[nodiscard]] std::string value() const {
    return { bencodeString.get(), BencodeStringLength};
  }
private:
  std::unique_ptr<char[]> bencodeString;
  std::size_t  BencodeStringLength {};
};

struct String : Variant {
  constexpr static int64_t kMaxLength = 16*1024;
  // Constructors/Destructors
  String() : Variant(Type::string) , bNodeString(""){}
  explicit String(const std::string &string)
      : Variant(Type::string), bNodeString(string) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Get BNode value
  [[nodiscard]] std::string value() { return bNodeString.value(); }
  [[nodiscard]] std::string value() const { return bNodeString.value(); }
  // Set/get maximum string length
  static void setMaxStringLength(const uint64_t length) { maxStringLength = length; }
  static uint64_t getMaxStringLength() { return maxStringLength; }
private:
  BencodeString bNodeString;
  inline static uint64_t maxStringLength { kMaxLength};
};
} // namespace Bencode_Lib