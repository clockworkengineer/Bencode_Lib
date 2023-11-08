#pragma once

#include <string>
#include <vector>

#include "Bencode_BNode.hpp"
#include "IDestination.hpp"
#include "IEncoder.hpp"

namespace Bencode_Lib {

class Encoder_Default : public IEncoder {

public:
  // Constructors/Destructors
  Encoder_Default() = default;
  Encoder_Default(const Encoder_Default &other) = delete;
  Encoder_Default &operator=(const Encoder_Default &other) = delete;
  Encoder_Default(Encoder_Default &&other) = delete;
  Encoder_Default &operator=(Encoder_Default &&other) = delete;
  virtual ~Encoder_Default() = default;

  void encode(const BNode &bNode, IDestination &destination) const;
};

} // namespace Bencode_Lib