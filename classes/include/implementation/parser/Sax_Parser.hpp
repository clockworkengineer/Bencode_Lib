// File: Sax_Parser.hpp
//
// Description: Streaming event-driven SAX parser for Bencode data.
//

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Status.hpp"
#include "interface/ISource.hpp"
#include "interface/ISaxHandler.hpp"
#include "../common/Bencode_Parser_Constants.hpp"

#include <cstdint>
#include <string_view>

namespace Bencode_Lib {

class SaxParser {
public:
  /// <summary>
  /// Parse Bencode data from an ISource stream, firing events on the provided ISaxHandler.
  /// Does not build an in-memory Node DOM tree, operating with O(depth) memory.
  /// </summary>
  /// <param name="source">Input source stream.</param>
  /// <param name="handler">SAX event handler callback interface.</param>
  /// <param name="maxDepth">Maximum allowed container nesting depth.</param>
  /// <returns>ParseStatus indicating success, user abort, or syntax error.</returns>
  static ParseStatus parse(ISource &source, ISaxHandler &handler,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);

  /// <summary>
  /// Parse Bencode data from a string_view buffer, firing events on the provided ISaxHandler.
  /// </summary>
  static ParseStatus parse(std::string_view bencodeString, ISaxHandler &handler,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);

#if BENCODE_ENABLE_EXCEPTIONS
  /// <summary>
  /// Parse Bencode data from an ISource stream, throwing on syntax errors.
  /// Returns false if the handler aborted parsing early, true if parsed to completion.
  /// </summary>
  static bool parseOrThrow(ISource &source, ISaxHandler &handler,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);

  /// <summary>
  /// Parse Bencode data from a string_view buffer, throwing on syntax errors.
  /// Returns false if the handler aborted parsing early, true if parsed to completion.
  /// </summary>
  static bool parseOrThrow(std::string_view bencodeString, ISaxHandler &handler,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);
#endif
};

} // namespace Bencode_Lib
