#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Parser final : public IParser {

public:
  constexpr static unsigned long kMaxParserDepth = 10;
  // Constructors/Destructors
  Bencode_Parser() = default;
  Bencode_Parser(const Bencode_Parser &other) = delete;
  Bencode_Parser &operator=(const Bencode_Parser &other) = delete;
  Bencode_Parser(Bencode_Parser &&other) = delete;
  Bencode_Parser &operator=(Bencode_Parser &&other) = delete;
  ~Bencode_Parser() override = default;
  // Parse bencode BNode tree
  BNode parse(ISource &source) override;
  // Get/Set parser max recursion depth
  static void setMaxParserDepth(const unsigned long depth) { maxParserDepth = depth; }
  static unsigned long getMaxParserDepth() { return maxParserDepth; }
private:
  // Parser functions
  [[nodiscard]] static int64_t extractInteger(ISource &source);
  [[nodiscard]] static BNode parseString(ISource &source,
                                         unsigned long parserDepth);
  [[nodiscard]] static BNode parseInteger(ISource &source,
                                          unsigned long parserDepth);
  [[nodiscard]] static BNode parseDictionary(ISource &source,
                                             unsigned long parserDepth);
  [[nodiscard]] static BNode parseList(ISource &source,
                                       unsigned long parserDepth);
  void static confirmBoundary(ISource &source, char expectedBoundary);
  [[nodiscard]] static BNode parseBNodes(ISource &source,
                                         unsigned long parserDepth);
  // Parser routing table
  using ParserFunc = std::function<BNode(ISource &, unsigned long)>;
  inline static std::map<char, ParserFunc> parsers = {
      {'d', parseDictionary}, {'l', parseList},   {'i', parseInteger},
      {'0', parseString},     {'1', parseString}, {'2', parseString},
      {'3', parseString},     {'4', parseString}, {'5', parseString},
      {'6', parseString},     {'7', parseString}, {'8', parseString},
      {'9', parseString}, {'-', parseString}, {'+', parseString}};
  inline  static unsigned long maxParserDepth { kMaxParserDepth };
};

} // namespace Bencode_Lib