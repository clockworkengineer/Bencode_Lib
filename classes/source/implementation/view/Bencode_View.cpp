// File: Bencode_View.cpp
//
// Description: Implementation of zero-copy non-owning Bencode parser and view types.
//

#include "Bencode.hpp"
#include "Bencode_View.hpp"
#include "interface/IParser.hpp"

#include <charconv>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace Bencode_Lib {

namespace {

// Fast skipper for already-validated Bencode elements
void skipValue(std::string_view input, size_t &pos) {
  const char c = input[pos];
  if (c == 'i') {
    pos++;
    while (input[pos] != 'e') {
      pos++;
    }
    pos++; // consume 'e'
  } else if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
    size_t len = 0;
    while (input[pos] != ':') {
      len = len * 10 + static_cast<size_t>(input[pos] - '0');
      pos++;
    }
    pos++; // consume ':'
    pos += len;
  } else if (c == 'l' || c == 'd') {
    pos++; // consume 'l' or 'd'
    size_t depth = 1;
    while (depth > 0) {
      if (input[pos] == 'l' || input[pos] == 'd') {
        depth++;
        pos++;
      } else if (input[pos] == 'e') {
        depth--;
        pos++;
      } else if (input[pos] == 'i') {
        pos++;
        while (input[pos] != 'e') {
          pos++;
        }
        pos++; // consume 'e'
      } else if (std::isdigit(static_cast<unsigned char>(input[pos])) != 0) {
        size_t len = 0;
        while (input[pos] != ':') {
          len = len * 10 + static_cast<size_t>(input[pos] - '0');
          pos++;
        }
        pos++; // consume ':'
        pos += len;
      }
    }
  }
}

} // namespace

// Fast syntax validation and element counting pass
ParseStatus BencodeView::scanElement(std::string_view input, size_t &pos, unsigned long depth,
                                    unsigned long maxDepth, BufferCounts &counts) {
  if (pos >= input.size()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source buffer terminated early.");
  }

  const char c = input[pos];

  if (c == 'i') {
    pos++;
    if (pos >= input.size()) {
      return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source buffer ended inside integer.");
    }
    const bool negative = (input[pos] == '-');
    if (negative) {
      pos++;
      if (pos >= input.size() || input[pos] == '0') {
        return ParseStatus::failure(ErrorCode::NegativeZero, "Negative zero is not allowed.");
      }
    }
    const size_t startDigits = pos;
    while (pos < input.size() && std::isdigit(static_cast<unsigned char>(input[pos])) != 0) {
      pos++;
    }
    if (pos == startDigits) {
      return ParseStatus::failure(ErrorCode::InvalidInteger, "Empty integer value.");
    }
    if (!negative && input[startDigits] == '0' && (pos - startDigits) > 1) {
      return ParseStatus::failure(ErrorCode::LeadingZero, "Leading zeros not allowed in integer.");
    }
    if (pos >= input.size() || input[pos] != 'e') {
      return ParseStatus::failure(ErrorCode::MissingEndTerminator, "Missing 'e' terminator for integer.");
    }
    // Verify integer fits in int64_t
    int64_t dummy = 0;
    auto [ptr, ec] = std::from_chars(input.data() + (negative ? startDigits - 1 : startDigits),
                                      input.data() + pos, dummy);
    if (ec != std::errc()) {
      return ParseStatus::failure(ErrorCode::IntegerOverflow, "Integer conversion overflow.");
    }
    pos++; // consume 'e'
    return ParseStatus::success();
  }

  if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
    const size_t startDigits = pos;
    while (pos < input.size() && std::isdigit(static_cast<unsigned char>(input[pos])) != 0) {
      pos++;
    }
    if (input[startDigits] == '0' && (pos - startDigits) > 1) {
      return ParseStatus::failure(ErrorCode::LeadingZero, "Leading zeros not allowed in string length.");
    }
    if (pos >= input.size() || input[pos] != ':') {
      return ParseStatus::failure(ErrorCode::MissingColon, "Missing colon separator in string value.");
    }
    size_t strLen = 0;
    auto [ptr, ec] = std::from_chars(input.data() + startDigits, input.data() + pos, strLen);
    if (ec != std::errc()) {
      return ParseStatus::failure(ErrorCode::IntegerOverflow, "String length overflow.");
    }
    pos++; // consume ':'
    if (pos + strLen > input.size()) {
      return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "String truncated before declared length.");
    }
    pos += strLen; // Jump directly over string bytes in zero-copy scan
    return ParseStatus::success();
  }

  if (c == 'l') {
    if (depth + 1 >= maxDepth) {
      return ParseStatus::failure(ErrorCode::MaximumParserDepthExceeded, "Maximum parser depth exceeded.");
    }
    pos++; // consume 'l'
    while (pos < input.size() && input[pos] != 'e') {
      counts.listElements++;
      ParseStatus elemStatus = scanElement(input, pos, depth + 1, maxDepth, counts);
      if (!elemStatus.ok()) {
        return elemStatus;
      }
    }
    if (pos >= input.size() || input[pos] != 'e') {
      return ParseStatus::failure(ErrorCode::MissingEndTerminator, "Missing 'e' terminator for list.");
    }
    pos++; // consume 'e'
    return ParseStatus::success();
  }

  if (c == 'd') {
    if (depth + 1 >= maxDepth) {
      return ParseStatus::failure(ErrorCode::MaximumParserDepthExceeded, "Maximum parser depth exceeded.");
    }
    pos++; // consume 'd'
    std::string_view lastKey{};
    while (pos < input.size() && input[pos] != 'e') {
      if (std::isdigit(static_cast<unsigned char>(input[pos])) == 0) {
        return ParseStatus::failure(ErrorCode::SyntaxError, "Dictionary key must be a string.");
      }
      const size_t startDigits = pos;
      while (pos < input.size() && std::isdigit(static_cast<unsigned char>(input[pos])) != 0) {
        pos++;
      }
      if (input[startDigits] == '0' && (pos - startDigits) > 1) {
        return ParseStatus::failure(ErrorCode::LeadingZero, "Leading zeros not allowed in string length.");
      }
      if (pos >= input.size() || input[pos] != ':') {
        return ParseStatus::failure(ErrorCode::MissingColon, "Missing colon separator in key string.");
      }
      size_t keyLen = 0;
      auto [ptr, ec] = std::from_chars(input.data() + startDigits, input.data() + pos, keyLen);
      if (ec != std::errc()) {
        return ParseStatus::failure(ErrorCode::IntegerOverflow, "Key string length overflow.");
      }
      pos++; // consume ':'
      if (pos + keyLen > input.size()) {
        return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Key string truncated.");
      }
      std::string_view key = input.substr(pos, keyLen);
      pos += keyLen;

      if (!lastKey.empty()) {
        if (key < lastKey) {
          return ParseStatus::failure(ErrorCode::DictionaryKeyOrder, "Dictionary keys not in sequence.");
        }
        if (key == lastKey) {
          return ParseStatus::failure(ErrorCode::DuplicateDictionaryKey, "Duplicate dictionary key.");
        }
      }
      lastKey = key;
      counts.dictEntries++;

      ParseStatus valStatus = scanElement(input, pos, depth + 1, maxDepth, counts);
      if (!valStatus.ok()) {
        return valStatus;
      }
    }
    if (pos >= input.size() || input[pos] != 'e') {
      return ParseStatus::failure(ErrorCode::MissingEndTerminator, "Missing 'e' terminator for dictionary.");
    }
    pos++; // consume 'e'
    return ParseStatus::success();
  }

  return ParseStatus::failure(ErrorCode::UnexpectedToken,
                              "Unexpected token '" + std::string(1, c) + "' at pos " +
                              std::to_string(pos));
}

// Pass 2: Constructs NodeView objects using pre-reserved contiguous storage
NodeView BencodeView::buildElement(std::string_view input, size_t &pos, BencodeView &view) {
  const char c = input[pos];

  if (c == 'i') {
    pos++; // consume 'i'
    const size_t startDigits = pos;
    while (input[pos] != 'e') {
      pos++;
    }
    int64_t val = 0;
    std::from_chars(input.data() + startDigits, input.data() + pos, val);
    pos++; // consume 'e'
    return NodeView(val);
  }

  if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
    const size_t startDigits = pos;
    while (input[pos] != ':') {
      pos++;
    }
    size_t strLen = 0;
    std::from_chars(input.data() + startDigits, input.data() + pos, strLen);
    pos++; // consume ':'
    std::string_view sv = input.substr(pos, strLen);
    pos += strLen;
    return NodeView(sv);
  }

  if (c == 'l') {
    pos++; // consume 'l'
    size_t count = 0;
    size_t scan = pos;
    while (input[scan] != 'e') {
      count++;
      skipValue(input, scan);
    }
    const size_t startIndex = view.listStorage.size();
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
    view.listStorage.resize(startIndex + count);
#else
    for (size_t i = 0; i < count; ++i) {
      view.listStorage.push_back(NodeView{});
    }
#endif
    for (size_t i = 0; i < count; ++i) {
      view.listStorage[startIndex + i] = buildElement(input, pos, view);
    }
    pos++; // consume 'e'
    return NodeView(std::span<const NodeView>(view.listStorage.data() + startIndex, count));
  }

  if (c == 'd') {
    pos++; // consume 'd'
    size_t count = 0;
    size_t scan = pos;
    while (input[scan] != 'e') {
      count++;
      skipValue(input, scan); // key
      skipValue(input, scan); // value
    }
    const size_t startIndex = view.dictStorage.size();
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
    view.dictStorage.resize(startIndex + count);
#else
    for (size_t i = 0; i < count; ++i) {
      view.dictStorage.push_back(DictEntry{});
    }
#endif
    for (size_t i = 0; i < count; ++i) {
      const size_t startDigits = pos;
      while (input[pos] != ':') {
        pos++;
      }
      size_t keyLen = 0;
      std::from_chars(input.data() + startDigits, input.data() + pos, keyLen);
      pos++; // consume ':'
      std::string_view key = input.substr(pos, keyLen);
      pos += keyLen;

      NodeView value = buildElement(input, pos, view);
      view.dictStorage[startIndex + i] = DictEntry{key, value};
    }
    pos++; // consume 'e'
    return NodeView(std::span<const DictEntry>(view.dictStorage.data() + startIndex, count));
  }

  return NodeView{};
}

ParseStatus BencodeView::parse(std::string_view rawBencode, BencodeView &destination,
                               unsigned long maxDepth) {
  if (rawBencode.empty()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Source buffer is empty.");
  }

  destination.rawBuffer = rawBencode;
  destination.rootNode = NodeView{};
  destination.listStorage.clear();
  destination.dictStorage.clear();

  // Pass 1: Syntax validation & exact allocation sizing
  BufferCounts counts;
  size_t scanPos = 0;
  ParseStatus status = scanElement(rawBencode, scanPos, 0, maxDepth, counts);
  if (!status.ok()) {
    return status;
  }
  if (scanPos < rawBencode.size()) {
    return ParseStatus::failure(ErrorCode::SourceTerminatedEarly, "Trailing bytes after Bencode payload.");
  }

#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
  destination.listStorage.reserve(counts.listElements);
  destination.dictStorage.reserve(counts.dictEntries);
#else
  if (counts.listElements > BENCODE_MAX_NODE_COUNT ||
      counts.dictEntries > BENCODE_MAX_CONTAINER_SIZE) {
    return ParseStatus::failure(ErrorCode::FixedVectorCapacityExceeded, "Buffer exceeds embedded capacity limits.");
  }
#endif

  // Pass 2: Construction with zero heap allocations for strings or keys
  size_t buildPos = 0;
  destination.rootNode = buildElement(rawBencode, buildPos, destination);

  return ParseStatus::success();
}

#if BENCODE_ENABLE_EXCEPTIONS
BencodeView BencodeView::parse(std::string_view rawBencode, unsigned long maxDepth) {
  BencodeView view;
  ParseStatus status = parse(rawBencode, view, maxDepth);
  if (!status.ok()) {
    throw IParser::Error(std::string("BencodeView Parse Error: ").append(status.message));
  }
  return view;
}
#endif

} // namespace Bencode_Lib
