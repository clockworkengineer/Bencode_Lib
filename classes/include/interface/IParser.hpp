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
#if BENCODE_ENABLE_EXCEPTIONS
  virtual Node parse(ISource &source) = 0;
  virtual ParseStatus parse(ISource &source, Node &destination) {
    destination = parse(source);
    return ParseStatus::success();
  }
#else
  virtual ParseStatus parse(ISource &source, Node &destination) = 0;
#endif
};
} // namespace Bencode_Lib