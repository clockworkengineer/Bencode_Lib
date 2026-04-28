// File: Bencode_Status.hpp
//
// Description: Status codes and result types used by parse/stringify operations throughout the library.
//

#pragma once

#include <string>

namespace Bencode_Lib {

enum class ErrorCode : int {
  None = 0,
  SyntaxError,
  UnexpectedToken,
  IntegerOverflow,
  InvalidInteger,
  LeadingZero,
  NegativeZero,
  NegativeStringLength,
  MissingColon,
  StringTooLong,
  DictionaryKeyOrder,
  DuplicateDictionaryKey,
  MissingEndTerminator,
  MaximumParserDepthExceeded,
  SourceTerminatedEarly,
  EmptyNode,
  NoBencodeToTraverse,
  UnknownNodeType,
  IOError,
  FileOpenFailure,
  FileWriteFailure,
  FixedVectorCapacityExceeded,
};

struct ParseStatus {
  ErrorCode code{ErrorCode::None};
  std::string message{};

  constexpr bool ok() const noexcept { return code == ErrorCode::None; }
  static ParseStatus success() { return ParseStatus{}; }
  static ParseStatus failure(const ErrorCode errorCode, std::string message) {
    return ParseStatus{errorCode, std::move(message)};
  }
};

} // namespace Bencode_Lib
