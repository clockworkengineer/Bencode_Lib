// File: Sax_Parser.cpp
//
// Description: Implementation of the streaming event-driven SAX parser for Bencode data.
//

#include "implementation/parser/Sax_Parser.hpp"
#include "implementation/parser/Default_Parser_Internal.hpp"
#include "implementation/io/Bencode_BufferSource.hpp"
#include "interface/IParser.hpp"

#include <array>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace Bencode_Lib {

namespace {

struct SaxFrame {
  enum class State : uint8_t {
    InList,
    DictExpectKey,
    DictExpectValue
  };
  ContainerType type;
  State state;
  std::string lastKey{};

  explicit SaxFrame(ContainerType t)
      : type(t),
        state(t == ContainerType::List ? State::InList : State::DictExpectKey),
        lastKey() {}
};

ParseStatus extractSaxInteger(ISource &source, Bencode::IntegerType &number) {
  std::array<char, std::numeric_limits<Bencode::IntegerType>::digits10 + 2> buffer{};
  std::size_t digits = 0;
  if (source.current() == ParserConstants::STRING_MINUS) {
    buffer[digits++] = source.current();
    source.next();
  }
  while (source.more() && std::isdigit(static_cast<unsigned char>(source.current())) != 0) {
    if (digits == buffer.size()) {
      return ParseStatus::failure(ErrorCode::IntegerOverflow, "Integer too large to fit in conversion buffer.");
    }
    buffer[digits++] = source.current();
    source.next();
  }
  if ((buffer[0] == '0' && digits > 1) || digits == 0) {
    return ParseStatus::failure(ErrorCode::LeadingZero, "Empty Integer or has leading zero.");
  }
  if (buffer[0] == '-' && digits == 2 && buffer[1] == '0') {
    return ParseStatus::failure(ErrorCode::NegativeZero, "Negative zero is not allowed.");
  }
  if (!convertToInteger(buffer.data(), digits, number)) {
    return ParseStatus::failure(ErrorCode::IntegerOverflow, "Integer conversion overflow.");
  }
  return ParseStatus::success();
}

ParseStatus extractSaxString(ISource &source, std::string &destination) {
  Bencode::IntegerType stringLength = 0;
  ParseStatus status = extractSaxInteger(source, stringLength);
  if (!status.ok()) {
    return status;
  }
  if (stringLength < 0) {
    return ParseStatus::failure(ErrorCode::NegativeStringLength, "Negative string length.");
  }
  if (!source.more() || source.current() != ParserConstants::COLON) {
    return ParseStatus::failure(ErrorCode::MissingColon, "Missing colon separator in string value.");
  }
  source.next();
  if (static_cast<uint64_t>(stringLength) > String::getMaxStringLength()) {
    return ParseStatus::failure(ErrorCode::StringTooLong, "String size exceeds maximum allowed size.");
  }
  destination.resize(static_cast<std::size_t>(stringLength));
  for (std::size_t i = 0; i < static_cast<std::size_t>(stringLength); ++i) {
    if (!source.more()) {
      return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source stream ended unexpectedly while reading string.");
    }
    destination[i] = source.current();
    source.next();
  }
  return ParseStatus::success();
}

} // namespace

ParseStatus SaxParser::parse(ISource &source, ISaxHandler &handler, unsigned long maxDepth) {
  if (!source.more()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source stream is empty.");
  }

  std::vector<SaxFrame> stack;
  stack.reserve(16);
  std::string stringBuffer;
  stringBuffer.reserve(128);

  auto handleValue = [&](char token) -> ParseStatus {
    if (token == ParserConstants::INTEGER) {
      source.next();
      Bencode::IntegerType intVal = 0;
      ParseStatus status = extractSaxInteger(source, intVal);
      if (!status.ok()) {
        return status;
      }
      if (!source.more() || source.current() != ParserConstants::END) {
        return ParseStatus::failure(ErrorCode::MissingEndTerminator, "Missing end terminator for integer.");
      }
      source.next();
      if (!handler.on_integer(intVal)) {
        return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
      }
      return ParseStatus::success();
    }
    if (std::isdigit(static_cast<unsigned char>(token)) != 0) {
      ParseStatus status = extractSaxString(source, stringBuffer);
      if (!status.ok()) {
        return status;
      }
      if (!handler.on_string(std::string_view(stringBuffer.data(), stringBuffer.size()))) {
        return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
      }
      return ParseStatus::success();
    }
    if (token == ParserConstants::LIST) {
      if (stack.size() + 1 >= maxDepth) {
        return ParseStatus::failure(ErrorCode::MaximumParserDepthExceeded, "Maximum parser depth exceeded.");
      }
      source.next();
      if (!handler.on_list_begin()) {
        return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
      }
      stack.emplace_back(ContainerType::List);
      return ParseStatus::success();
    }
    if (token == ParserConstants::DICTIONARY) {
      if (stack.size() + 1 >= maxDepth) {
        return ParseStatus::failure(ErrorCode::MaximumParserDepthExceeded, "Maximum parser depth exceeded.");
      }
      source.next();
      if (!handler.on_dictionary_begin()) {
        return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
      }
      stack.emplace_back(ContainerType::Dictionary);
      return ParseStatus::success();
    }
    return ParseStatus::failure(ErrorCode::UnexpectedToken, "Unexpected token in Bencode input.");
  };

  // Top-level root element
  char rootToken = source.current();
  ParseStatus rootStatus = handleValue(rootToken);
  if (!rootStatus.ok()) {
    return rootStatus;
  }

  // Iterative parsing loop for containers
  while (!stack.empty()) {
    if (!source.more()) {
      return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source stream ended unexpectedly inside container.");
    }

    auto &top = stack.back();

    if (top.type == ContainerType::List) {
      if (source.current() == ParserConstants::END) {
        source.next();
        if (!handler.on_list_end()) {
          return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
        }
        stack.pop_back();
        continue;
      }
      ParseStatus elemStatus = handleValue(source.current());
      if (!elemStatus.ok()) {
        return elemStatus;
      }
    } else { // ContainerType::Dictionary
      if (top.state == SaxFrame::State::DictExpectKey) {
        if (source.current() == ParserConstants::END) {
          source.next();
          if (!handler.on_dictionary_end()) {
            return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
          }
          stack.pop_back();
          continue;
        }

        if (std::isdigit(static_cast<unsigned char>(source.current())) == 0) {
          return ParseStatus::failure(ErrorCode::SyntaxError, "Dictionary key must be a string.");
        }

        std::string keyBuffer;
        ParseStatus keyStatus = extractSaxString(source, keyBuffer);
        if (!keyStatus.ok()) {
          return keyStatus;
        }

        if (!top.lastKey.empty()) {
          if (keyBuffer < top.lastKey) {
            return ParseStatus::failure(ErrorCode::DictionaryKeyOrder, "Dictionary keys not in sequence.");
          }
          if (keyBuffer == top.lastKey) {
            return ParseStatus::failure(ErrorCode::DuplicateDictionaryKey, "Duplicate dictionary key.");
          }
        }
        top.lastKey = keyBuffer;
        top.state = SaxFrame::State::DictExpectValue;

        if (!handler.on_dictionary_key(keyBuffer)) {
          return ParseStatus::failure(ErrorCode::UserAborted, "Parsing cancelled by handler.");
        }
      } else { // DictExpectValue
        top.state = SaxFrame::State::DictExpectKey;
        ParseStatus valStatus = handleValue(source.current());
        if (!valStatus.ok()) {
          return valStatus;
        }
      }
    }
  }

  if (source.more()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source stream contains unexpected trailing data.");
  }

  return ParseStatus::success();
}

ParseStatus SaxParser::parse(std::string_view bencodeString, ISaxHandler &handler, unsigned long maxDepth) {
  if (bencodeString.empty()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source buffer is empty.");
  }
  BufferSource source{bencodeString};
  return parse(source, handler, maxDepth);
}

#if BENCODE_ENABLE_EXCEPTIONS
bool SaxParser::parseOrThrow(ISource &source, ISaxHandler &handler, unsigned long maxDepth) {
  ParseStatus status = parse(source, handler, maxDepth);
  if (status.code == ErrorCode::UserAborted) {
    return false;
  }
  if (!status.ok()) {
    throw IParser::Error(std::string("Bencode Syntax Error: ").append(status.message));
  }
  return true;
}

bool SaxParser::parseOrThrow(std::string_view bencodeString, ISaxHandler &handler, unsigned long maxDepth) {
  if (bencodeString.empty()) {
    throw IParser::Error("Bencode Syntax Error: Source buffer is empty.");
  }
  BufferSource source{bencodeString};
  return parseOrThrow(source, handler, maxDepth);
}
#endif

} // namespace Bencode_Lib
