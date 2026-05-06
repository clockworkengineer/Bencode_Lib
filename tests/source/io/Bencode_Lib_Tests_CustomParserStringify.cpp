#include "Bencode_Lib_Tests.hpp"
#include <cctype>
#include <string>

namespace {

class CustomParser final : public IParser {
protected:
#if BENCODE_ENABLE_EXCEPTIONS
  Node parseImpl(ISource &source) override {
    if (!source.more() || source.current() != 'i') {
      throw Error("Expected integer prefix.");
    }
    source.next();

    std::string digits;
    while (source.more() && std::isdigit(static_cast<unsigned char>(source.current()))) {
      digits.push_back(source.current());
      source.next();
    }

    if (!source.more() || source.current() != 'e') {
      throw Error("Expected integer terminator.");
    }
    source.next();

    if (digits.empty()) {
      throw Error("Integer value missing.");
    }
    return Node(std::stoll(digits));
  }
#else
  ParseStatus parseImpl(ISource &source, Node &destination) override {
    if (!source.more() || source.current() != 'i') {
      return ParseStatus::failure(ErrorCode::SyntaxError,
                                  "Expected integer prefix.");
    }
    source.next();

    std::string digits;
    while (source.more() && std::isdigit(static_cast<unsigned char>(source.current()))) {
      digits.push_back(source.current());
      source.next();
    }

    if (!source.more() || source.current() != 'e') {
      return ParseStatus::failure(ErrorCode::SyntaxError,
                                  "Expected integer terminator.");
    }
    source.next();

    if (digits.empty()) {
      return ParseStatus::failure(ErrorCode::SyntaxError,
                                  "Integer value missing.");
    }
    destination = Node(std::stoll(digits));
    return ParseStatus::success();
  }
#endif
};

class CustomStringify final : public IStringify {
public:
  void stringify(const Node &bNode, IDestination &destination) const override {
    destination.add("custom:");
    if (isA<Integer>(bNode)) {
      destination.add(std::to_string(NRef<Integer>(bNode).value()));
    } else if (isA<String>(bNode)) {
      destination.add(NRef<String>(bNode).value());
    } else {
      destination.add("<unsupported>");
    }
  }
};

class StringDestination final : public IDestination {
public:
  void add(const std::string &bytes) override { buffer.append(bytes); }
  void add(const std::string_view &bytes) override { buffer.append(bytes); }
  void add(const char *bytes) override { buffer.append(bytes); }
  void add(char ch) override { buffer.push_back(ch); }
  void clear() override { buffer.clear(); }
  char last() override { return buffer.empty() ? '\0' : buffer.back(); }
  const std::string &str() const noexcept { return buffer; }

private:
  std::string buffer;
};

} // namespace

TEST_CASE("Custom IParser implementation is invoked by Bencode",
          "[Bencode][IParser][Integration]") {
  auto *parser = new CustomParser();
  Bencode b(nullptr, parser);
  b.parse(BufferSource("i73e"));

  REQUIRE(isA<Integer>(b.root()));
  REQUIRE(NRef<Integer>(b.root()).value() == 73);
}

TEST_CASE("Custom IStringify implementation is invoked by Bencode",
          "[Bencode][IStringify][Integration]") {
  Bencode b(makeStringify<CustomStringify>(), nullptr);
  b.root() = Node(123);

  StringDestination destination;
  b.stringify(destination);

  REQUIRE(destination.str() == "custom:123");
}
