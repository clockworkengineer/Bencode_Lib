// File: Default_Parser_Internal.hpp
//
// Description: Shared parser helper definitions used by the default Bencode
// parser implementation.

#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Parser_Constants.hpp"

#include <array>
#include <limits>
#include <string>
#include <string_view>

namespace Bencode_Lib {

enum class ContainerType { List, Dictionary };

struct ParserFrame {
  ContainerType type;
  Node container;
  std::string lastKey;
  std::string currentKey;
  bool awaitingValue = false;

  explicit ParserFrame(ContainerType frameType)
      : type(frameType),
        container(frameType == ContainerType::List ? Node::make<List>()
                                                   : Node::make<Dictionary>()),
        lastKey(),
        currentKey(),
        awaitingValue(false) {
    lastKey.reserve(64);
    currentKey.reserve(64);
  }
};

inline void copySourceToBuffer(ISource &source, char *buffer,
                               const std::size_t length) {
  for (std::size_t index = 0; index < length; ++index) {
    buffer[index] = source.current();
    source.next();
  }
}

inline bool convertToInteger(const char *buffer, const std::size_t digits,
                             Bencode::IntegerType &value) {
  const bool negative =
      digits > 0 && buffer[0] == ParserConstants::STRING_MINUS;
  std::size_t start = negative ? 1 : 0;
  unsigned long long limit = static_cast<unsigned long long>(
      std::numeric_limits<Bencode::IntegerType>::max());
  if (negative) {
    limit += 1ull;
  }
  unsigned long long result = 0ull;
  for (std::size_t index = start; index < digits; ++index) {
    unsigned int digit = static_cast<unsigned int>(buffer[index] - '0');
    if (result > (limit - digit) / 10ull) {
      return false;
    }
    result = (result * 10ull) + digit;
  }
  if (negative) {
    value = -static_cast<Bencode::IntegerType>(result);
  } else {
    value = static_cast<Bencode::IntegerType>(result);
  }
  return true;
}

inline ParseStatus attachCompletedValue(ParserFrame &parent, Node &&completed) {
  if (parent.type == ContainerType::List) {
    NRef<List>(parent.container).add(std::move(completed));
    return ParseStatus::success();
  }
  if (!parent.awaitingValue) {
    return ParseStatus::failure(ErrorCode::SyntaxError,
                                "Dictionary value missing key.");
  }
  NRef<Dictionary>(parent.container)
      .appendSorted(Dictionary::Entry(std::move(parent.currentKey),
                                      std::move(completed)));
  parent.awaitingValue = false;
  return ParseStatus::success();
}

} // namespace Bencode_Lib
