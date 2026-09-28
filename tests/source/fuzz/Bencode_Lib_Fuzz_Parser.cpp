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

#if defined(__has_include)
#if __has_include(<unistd.h>)
#include <unistd.h>
#define HAS_UNISTD 1
#endif
#endif

int main(int argc, char **argv) {
  std::string input;

  if (argc > 1) {
    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
      return 1;
    }
    std::istreambuf_iterator<char> it(file);
    std::istreambuf_iterator<char> end;
    input.assign(it, end);
  } else {
#if defined(HAS_UNISTD)
    if (isatty(STDIN_FILENO)) {
      input = "d4:spaml1:a1:bee";
    } else {
      std::istreambuf_iterator<char> it(std::cin);
      std::istreambuf_iterator<char> end;
      input.assign(it, end);
      if (input.empty()) {
        input = "d4:spaml1:a1:bee";
      }
    }
#else
    input = "d4:spaml1:a1:bee";
#endif
  }

  parseFuzzInput(std::string_view(input.data(), input.size()));
  return 0;
}
