// File: Bencode_Hole.hpp
//
// Description: Placeholder variant representing an empty or uninitialized Bencode node value.
//

#pragma once

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