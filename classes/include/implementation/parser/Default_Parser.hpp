#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Parser_Constants.hpp"

namespace Bencode_Lib {

class Default_Parser final : public IParser {

public:
  constexpr static unsigned long kMaxParserDepth =
      ParserConstants::DEFAULT_MAX_PARSER_DEPTH;
  // Constructors/Destructors
  Default_Parser() = default;
  Default_Parser(const Default_Parser &other) = delete;
  Default_Parser &operator=(const Default_Parser &other) = delete;
  Default_Parser(Default_Parser &&other) = delete;
  Default_Parser &operator=(Default_Parser &&other) = delete;
  ~Default_Parser() override = default;
  // Parse bencode Node tree
  Node parse(ISource &source) override;
  // Get/Set parser max recursion depth
  static void setMaxParserDepth(const unsigned long depth) {
    maxParserDepth = depth;
  }
  static unsigned long getMaxParserDepth() { return maxParserDepth; }

private:
  // Parser functions
  [[nodiscard]] static Bencode::IntegerType extractInteger(ISource &source);
  [[nodiscard]] static Node parseString(ISource &source,
                                        unsigned long parserDepth);
  [[nodiscard]] static Node parseInteger(ISource &source,
                                         unsigned long parserDepth);
  [[nodiscard]] static Node parseDictionary(ISource &source,
                                            unsigned long parserDepth);
  [[nodiscard]] static Node parseList(ISource &source,
                                      unsigned long parserDepth);
  void static confirmBoundary(ISource &source, char expectedBoundary);
  [[nodiscard]] static Node parseNodes(ISource &source,
                                       unsigned long parserDepth);
  // Parser routing table
  using ParserFunc = std::function<Node(ISource &, unsigned long)>;
  inline static std::map<char, ParserFunc> parsers = {
      {ParserConstants::DICTIONARY, parseDictionary},
      {ParserConstants::LIST, parseList},
      {ParserConstants::INTEGER, parseInteger},
      {ParserConstants::STRING_0, parseString},
      {ParserConstants::STRING_1, parseString},
      {ParserConstants::STRING_2, parseString},
      {ParserConstants::STRING_3, parseString},
      {ParserConstants::STRING_4, parseString},
      {ParserConstants::STRING_5, parseString},
      {ParserConstants::STRING_6, parseString},
      {ParserConstants::STRING_7, parseString},
      {ParserConstants::STRING_8, parseString},
      {ParserConstants::STRING_9, parseString},
      {ParserConstants::STRING_MINUS, parseString},
      {ParserConstants::STRING_PLUS, parseString}};
  inline static unsigned long maxParserDepth{kMaxParserDepth};
};

} // namespace Bencode_Lib