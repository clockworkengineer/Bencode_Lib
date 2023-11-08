#pragma once

#include <string>
#include <vector>

#include "Bencode_BNode.hpp"
#include "ISource.hpp"
#include "IDecoder.hpp"

namespace Bencode_Lib {

class Decoder_Default : public IDecoder {

public:
  // Constructors/Destructors
  Decoder_Default() = default;
  Decoder_Default(const Decoder_Default &other) = delete;
  Decoder_Default &operator=(const Decoder_Default &other) = delete;
  Decoder_Default(Decoder_Default &&other) = delete;
  Decoder_Default &operator=(Decoder_Default &&other) = delete;
  virtual ~Decoder_Default() = default;

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