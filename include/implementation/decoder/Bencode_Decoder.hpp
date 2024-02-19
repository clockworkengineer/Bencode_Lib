#pragma once

#include <string>
#include <vector>
#include <array>
#include <limits>

#include "Bencode_Core.hpp"

namespace Bencode_Lib {

class Bencode_Decoder : public IDecoder {

public:
  // Constructors/Destructors
  Bencode_Decoder() = default;
  Bencode_Decoder(const Bencode_Decoder &other) = delete;
  Bencode_Decoder &operator=(const Bencode_Decoder &other) = delete;
  Bencode_Decoder(Bencode_Decoder &&other) = delete;
  Bencode_Decoder &operator=(Bencode_Decoder &&other) = delete;
  virtual ~Bencode_Decoder() = default;

  BNode decode(ISource &source);

private:
  int64_t extractInteger(ISource &source);
  std::string extractString(ISource &source);
  BNode decodeString(ISource &source);
  BNode decodeInteger(ISource &source);
  BNode decodeDictionary(ISource &source);
  BNode decodeList(ISource &source);
};

} // namespace Bencode_Lib