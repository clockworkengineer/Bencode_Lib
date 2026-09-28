// File: ISaxHandler.hpp
//
// Description: Interface defining event-driven callbacks for streaming Bencode SAX parsing.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace Bencode_Lib {

/// <summary>
/// Abstract handler interface for event-driven Bencode streaming parsing.
/// Implementations return false from any event callback to stop parsing early.
/// </summary>
class ISaxHandler {
public:
  virtual ~ISaxHandler() = default;

  /// <summary>
  /// Called when an integer value is encountered.
  /// </summary>
  /// <param name="value">Parsed 64-bit integer.</param>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_integer(int64_t value) = 0;

  /// <summary>
  /// Called when a string or byte string value is encountered.
  /// </summary>
  /// <param name="value">String view over the parsed byte payload.</param>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_string(std::string_view value) = 0;

  /// <summary>
  /// Optional hook called for binary byte payloads.
  /// Default implementation delegates directly to on_string.
  /// </summary>
  /// <param name="bytes">Span of const std::byte viewing the raw payload.</param>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_binary(std::span<const std::byte> bytes) {
    return on_string(std::string_view(reinterpret_cast<const char *>(bytes.data()), bytes.size()));
  }

  /// <summary>
  /// Called when a list begins ('l').
  /// </summary>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_list_begin() = 0;

  /// <summary>
  /// Called when a list ends ('e').
  /// </summary>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_list_end() = 0;

  /// <summary>
  /// Called when a dictionary begins ('d').
  /// </summary>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_dictionary_begin() = 0;

  /// <summary>
  /// Called for each dictionary key before parsing its associated value.
  /// </summary>
  /// <param name="key">Dictionary key as a string_view.</param>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_dictionary_key(std::string_view key) = 0;

  /// <summary>
  /// Called when a dictionary ends ('e').
  /// </summary>
  /// <returns>True to continue parsing, false to abort.</returns>
  virtual bool on_dictionary_end() = 0;
};

} // namespace Bencode_Lib
