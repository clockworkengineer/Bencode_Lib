// Program: Bencode_Nested_Structures_Example
// Description: Demonstrates encoding and decoding nested lists and dictionaries
// with Bencode_Lib. Dependencies: C++20, Bencode_Lib.

#include "Bencode.hpp"
#include "Bencode_Core.hpp"
#include <iostream>

namespace be = Bencode_Lib;

int main() {
  // Create a nested dictionary with a list using supported initializer types
  be::Bencode::DictionaryInitializerType dict = {
      {"user", std::string("alice")},
      {"roles", be::Bencode::ListInitializerType{std::string("admin"),
                                                 std::string("editor")}},
      {"profile", be::Bencode::DictionaryInitializerType{
                      {"age", 28}, {"active", std::string("yes")}}}};
  be::Bencode doc(dict);

  // Encode
  be::BufferDestination dest;
  doc.stringify(dest);
  std::string encoded = dest.toString();
  std::cout << "Bencoded nested structure: " << encoded << std::endl;

  // Decode
  be::Bencode decoded(encoded);
  std::cout << "Decoded user: " << be::NRef<be::String>(decoded["user"]).value()
            << std::endl;
  std::cout << "Decoded profile age: "
            << be::NRef<be::Integer>(decoded["profile"]["age"]).value()
            << std::endl;
  return 0;
}
