// File: Bencode_View.hpp
//
// Description: Zero-copy non-owning Bencode parser and view types (BencodeView, NodeView).
//

#pragma once

#include "Bencode_Config.hpp"
#include "Bencode_Status.hpp"
#include "implementation/view/Bencode_NodeView.hpp"
#include "implementation/common/Bencode_Parser_Constants.hpp"
#if !BENCODE_ENABLE_DYNAMIC_ALLOCATION
#include "implementation/variants/Bencode_FixedVector.hpp"
#endif

#include <cstddef>
#include <string_view>
#include <vector>

namespace Bencode_Lib {

class BencodeView {
public:
  using DictEntry = Bencode_Lib::DictEntry;

  using NodeVector =
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
      std::vector<NodeView>;
#else
      FixedVector<NodeView, BENCODE_MAX_NODE_COUNT>;
#endif

  using DictEntryVector =
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
      std::vector<DictEntry>;
#else
      FixedVector<DictEntry, BENCODE_MAX_CONTAINER_SIZE>;
#endif

  BencodeView() = default;
  BencodeView(const BencodeView &) = delete;
  BencodeView &operator=(const BencodeView &) = delete;
  BencodeView(BencodeView &&) noexcept = default;
  BencodeView &operator=(BencodeView &&) noexcept = default;
  ~BencodeView() = default;

#if BENCODE_ENABLE_EXCEPTIONS
  /// <summary>
  /// Parse raw Bencode payload into a zero-copy BencodeView tree.
  /// Throws IParser::Error on invalid syntax.
  /// The input string_view buffer MUST outlive the returned BencodeView.
  /// </summary>
  static BencodeView parse(std::string_view rawBencode,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);
#endif

  /// <summary>
  /// Parse raw Bencode payload into destination BencodeView returning ParseStatus.
  /// Does not throw exceptions.
  /// </summary>
  static ParseStatus parse(std::string_view rawBencode, BencodeView &destination,
                           unsigned long maxDepth = ParserConstants::DEFAULT_MAX_PARSER_DEPTH);

  [[nodiscard]] const NodeView &root() const noexcept { return rootNode; }
  [[nodiscard]] NodeView operator[](std::string_view key) const { return rootNode[key]; }
  [[nodiscard]] NodeView operator[](std::size_t index) const { return rootNode[index]; }

  // Object mapping template methods
  template <typename T>
  [[nodiscard]] T as() const;
  template <typename T>
  [[nodiscard]] T get() const;

  [[nodiscard]] std::string_view raw() const noexcept { return rawBuffer; }

private:
  struct BufferCounts {
    std::size_t listElements = 0;
    std::size_t dictEntries = 0;
  };

  static ParseStatus scanElement(std::string_view input, size_t &pos, unsigned long depth,
                                 unsigned long maxDepth, BufferCounts &counts);
  static NodeView buildElement(std::string_view input, size_t &pos, BencodeView &view);

  std::string_view rawBuffer{};
  NodeView rootNode{};
  NodeVector listStorage{};
  DictEntryVector dictStorage{};
};

} // namespace Bencode_Lib
