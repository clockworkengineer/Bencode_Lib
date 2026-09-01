// File: Default_Parser.hpp
//
// Description: Header declaring the default Bencode parser implementation for decoding input streams.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "../common/Bencode_Parser_Constants.hpp"

#include "Parser_Config.hpp"

namespace Bencode_Lib {

class Default_Parser final : public IParser {

public:
  constexpr static unsigned long kMaxParserDepth =
      ParserConstants::DEFAULT_MAX_PARSER_DEPTH;
  // Constructors/Destructors
  explicit Default_Parser(ParserConfig config = {}) : parserConfig(config) {}
  Default_Parser(const Default_Parser &other) = delete;
  Default_Parser &operator=(const Default_Parser &other) = delete;
  Default_Parser(Default_Parser &&other) = delete;
  Default_Parser &operator=(Default_Parser &&other) = delete;
  ~Default_Parser() override = default;
  // Parse bencode Node tree
#if BENCODE_ENABLE_EXCEPTIONS
  Node parseImpl(ISource &source) override;
#else
  ParseStatus parseImpl(ISource &source, Node &destination) override;
#endif
  // Get/Set parser max recursion depth
  static void setMaxParserDepth(const unsigned long depth) {
    defaultConfig.maxParserDepth = depth;
  }
  static unsigned long getMaxParserDepth() { return defaultConfig.maxParserDepth; }

  [[nodiscard]] const ParserConfig &getConfig() const { return parserConfig; }
  void setConfig(const ParserConfig &config) { parserConfig = config; }

private:
  // Parser functions
#if BENCODE_ENABLE_EXCEPTIONS
  [[nodiscard]] static Bencode::IntegerType extractInteger(ISource &source);
  [[nodiscard]] static Node parseString(ISource &source,
                                        unsigned long parserDepth);
  [[nodiscard]] static std::string parseStringKey(ISource &source,
                                                  unsigned long parserDepth);
  static void parseStringKey(ISource &source, unsigned long parserDepth,
                             std::string &destination);
  [[nodiscard]] static Node parseInteger(ISource &source,
                                         unsigned long parserDepth);
  [[nodiscard]] static Node parseDictionary(ISource &source,
                                            unsigned long parserDepth);
  [[nodiscard]] static Node parseList(ISource &source,
                                      unsigned long parserDepth);
  [[nodiscard]] static Node parseScalar(ISource &source,
                                        unsigned long parserDepth);
  void static confirmBoundary(ISource &source, char expectedBoundary);
  [[nodiscard]] static Node parseNodes(ISource &source,
                                       unsigned long parserDepth);
  [[nodiscard]] static Node parseIterative(ISource &source);
#else
  [[nodiscard]] static ParseStatus extractInteger(ISource &source,
                                                  Bencode::IntegerType &value);
  [[nodiscard]] static ParseStatus
  parseString(ISource &source, unsigned long parserDepth, Node &destination);
  [[nodiscard]] static ParseStatus parseStringKey(ISource &source,
                                                  unsigned long parserDepth,
                                                  std::string &destination);
  [[nodiscard]] static ParseStatus
  parseInteger(ISource &source, unsigned long parserDepth, Node &destination);
  [[nodiscard]] static ParseStatus parseDictionary(ISource &source,
                                                   unsigned long parserDepth,
                                                   Node &destination);
  [[nodiscard]] static ParseStatus
  parseList(ISource &source, unsigned long parserDepth, Node &destination);
  [[nodiscard]] static ParseStatus
  parseScalar(ISource &source, unsigned long parserDepth, Node &destination);
  [[nodiscard]] static ParseStatus confirmBoundary(ISource &source,
                                                   char expectedBoundary);
  [[nodiscard]] static ParseStatus
  parseNodes(ISource &source, unsigned long parserDepth, Node &destination);
  [[nodiscard]] static ParseStatus parseIterative(ISource &source,
                                                  Node &destination);
#endif
  ParserConfig parserConfig{};
  inline static ParserConfig defaultConfig{};
};

} // namespace Bencode_Lib