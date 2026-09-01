// File: Parser_Config.hpp
//
// Description: Configuration structure for Bencode parser options.
//

#pragma once

#include "../common/Bencode_Parser_Constants.hpp"

namespace Bencode_Lib {

struct ParserConfig {
  unsigned long maxParserDepth{ParserConstants::DEFAULT_MAX_PARSER_DEPTH};
};

} // namespace Bencode_Lib
