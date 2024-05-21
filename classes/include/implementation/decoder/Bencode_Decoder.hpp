#pragma once

#include <string>
#include <vector>
#include <array>
#include <limits>

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

  BNode decode(ISource &source) override;

private:
  static int64_t extractInteger(ISource &source);
  static std::string extractString(ISource &source);
  static BNode decodeString(ISource &source);
  static BNode decodeInteger(ISource &source);
  BNode decodeDictionary(ISource &source);
  BNode decodeList(ISource &source);
};

} // namespace Bencode_Lib