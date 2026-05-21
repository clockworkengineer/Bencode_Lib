#include "Bencode.hpp"
#include "implementation/io/Bencode_BufferSource.hpp"
#include "interface/IParser.hpp"
#include "interface/IStringify.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

using namespace Bencode_Lib;

static void parseFuzzInput(const std::string_view data) {
  if (data.empty()) {
    return;
  }

  try {
    Bencode parser;
    BufferSource source{data};
    parser.parse(source);
  } catch (const std::exception &) {
    // Parsing failures are expected for malformed input.
  } catch (...) {
    // Catch all non-standard exceptions to avoid harness termination.
  }
}

int main(int argc, char **argv) {
  std::string input;

  if (argc == 1) {
    std::istreambuf_iterator<char> it(std::cin);
    std::istreambuf_iterator<char> end;
    input.assign(it, end);
  } else {
    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
      return 1;
    }
    std::istreambuf_iterator<char> it(file);
    std::istreambuf_iterator<char> end;
    input.assign(it, end);
  }

  parseFuzzInput(std::string_view(input.data(), input.size()));
  return 0;
}
