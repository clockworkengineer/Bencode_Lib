// File: IParser.hpp
//
// Description: Interface defining Bencode parser behavior for decoding from a source.
//

#pragma once

#include "implementation/common/Bencode_Status.hpp"
#include "implementation/node/Bencode_Node.hpp"

namespace Bencode_Lib {

// ====================
// Forward declarations
// ====================
class ISource;

// =============================
// Interface for Bencode parser
// =============================
class IParser {
public:
  // ==============
  // IParser Error
  // ==============
  struct Error final : std::runtime_error {
    explicit Error(const std::string_view &message)
        : std::runtime_error(std::string("IParser Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IParser() = default;
  // ==========================================
  // Parse Bencode into Node tree from source
  // ==========================================
  virtual Node parse(ISource &source) {
#if !BENCODE_ENABLE_EXCEPTIONS
    Node destination;
    ParseStatus status = parse(source, destination);
    if (!status.ok()) {
      throw Error("IParser parse failed.");
    }
    return destination;
#else
    return parseImpl(source);
#endif
  }

  virtual ParseStatus parse(ISource &source, Node &destination) {
#if BENCODE_ENABLE_EXCEPTIONS
    destination = parse(source);
    return ParseStatus::success();
#else
    return parseImpl(source, destination);
#endif
  }

protected:
#if BENCODE_ENABLE_EXCEPTIONS
  virtual Node parseImpl(ISource &source) = 0;
#else
  virtual ParseStatus parseImpl(ISource &source, Node &destination) = 0;
#endif
};
} // namespace Bencode_Lib