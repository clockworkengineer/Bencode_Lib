#pragma once

#include <string>
namespace Bencode_Lib {
    
struct Hole : Variant {
  // Constructors/Destructors
  Hole() : Variant(Type::hole) {}
  Hole(const Hole &other) = default;
  Hole &operator=(const Hole &other) = default;
  Hole(Hole &&other) = default;
  Hole &operator=(Hole &&other) = default;
  ~Hole() = default;
};
} // namespace Bencode_Lib