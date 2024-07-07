#pragma once

namespace Bencode_Lib {

struct STR {
  STR(const std::string &str) {
    pStr.reset(static_cast<char *>(malloc(str.size())));
    for (std::size_t i = 0; i < str.size(); i++) {
      pStr[i] = str[i];
    }
    len = str.size();
  }
  std::string value() {
    return std::string(pStr.get(), len);
  }
  const std::string value() const {
    return std::string(pStr.get(), len);
  }
private:
  std::unique_ptr<char[]> pStr;
  std::size_t  len {};
};
struct String : Variant {
  // Constructors/Destructors
  String() : Variant(Type::string) , bNodeString(""){}
  explicit String(std::string string)
      : Variant(Type::string), bNodeString(string) {}
  String(const String &other) = delete;
  String &operator=(const String &other) = delete;
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  ~String() = default;
  // Get BNode value
  [[nodiscard]] std::string value() { return bNodeString.value(); }
  [[nodiscard]] const std::string value() const { return bNodeString.value(); }

private:
  STR bNodeString;
};
} // namespace Bencode_Lib