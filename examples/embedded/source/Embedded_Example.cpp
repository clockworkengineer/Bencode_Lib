#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include <iostream>
#include <string>

using namespace Bencode_Lib;

int main() {
  const std::string data = "d3:key5:valuee";
  Bencode document;

#if BENCODE_ENABLE_EXCEPTIONS
  document.parse(BufferSource{data});
#else
  ParseStatus status = document.parse(BufferSource{data});
  if (!status.ok()) {
    std::cerr << "Parse failed: " << status.message() << std::endl;
    return 1;
  }
#endif

  BufferDestination destination;
  document.stringify(destination);
  std::cout << destination.toString() << std::endl;
  return 0;
}
