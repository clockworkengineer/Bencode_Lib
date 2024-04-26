#pragma once

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include "Bencode_Translator.hpp"

namespace Bencode_Lib {

class Bencode_Encoder : public IEncoder {

public:
  // Constructors/Destructors
  Bencode_Encoder() = default;
  Bencode_Encoder(const Bencode_Encoder &other) = delete;
  Bencode_Encoder &operator=(const Bencode_Encoder &other) = delete;
  Bencode_Encoder(Bencode_Encoder &&other) = delete;
  Bencode_Encoder &operator=(Bencode_Encoder &&other) = delete;
  virtual ~Bencode_Encoder() = default;

  void encode(const BNode &bNode, IDestination &destination) const;

private:
  Bencode_Translator bencodeTranslator;
};

} // namespace Bencode_Lib