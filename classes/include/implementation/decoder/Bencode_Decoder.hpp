#pragma once

#include <string>
#include <array>
#include <limits>
#include <functional>
#include <map>

#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Decoder final : public IDecoder {

public:
  // Constructors/Destructors
  Bencode_Decoder() = default;
  Bencode_Decoder(const Bencode_Decoder &other) = delete;
  Bencode_Decoder &operator=(const Bencode_Decoder &other) = delete;
  Bencode_Decoder(Bencode_Decoder &&other) = delete;
  Bencode_Decoder &operator=(Bencode_Decoder &&other) = delete;
  ~Bencode_Decoder() override = default;
  // Decode bencode BNode tree
  BNode decode(ISource &source) override;

private:
  [[nodiscard]] static int64_t extractInteger(ISource &source);
  [[nodiscard]] static std::string extractString(ISource &source);
  [[nodiscard]] static BNode decodeString(ISource &source);
  [[nodiscard]] static BNode decodeInteger(ISource &source);
  [[nodiscard]] static BNode decodeDictionary(ISource &source);
  [[nodiscard]] static BNode decodeList(ISource &source);
  void static confirmBoundary(ISource &source, char expectedBoundary);
  [[nodiscard]] static BNode decodeTree(ISource &source);
};

} // namespace Bencode_Lib