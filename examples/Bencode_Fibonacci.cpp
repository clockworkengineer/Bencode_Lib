//
// Program: Bencode_Fibonacci
//
// Description: On each activation add the next entry in the fibonacci
// sequence to the array stored in fibonacci.ben; if the file does
// not exist then create the initial sequence of li0ei1ee.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Utility.hpp"

#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace be = Bencode_Lib;

/// <summary>
/// Return Fibonaci bEncode file name.
/// </summary>
/// <returns>Bencode settings file name.</returns>
std::string bEncodeFibonacciFile() {
  return (
      (std::filesystem::current_path() / "files" / "fibonacci.ben").string());
}
/// <summary>
/// Read in current fibonacci sequence from Bencode file, calculate the
//  next in sequence and write back to Bencode file.
/// </summary>
void nextFibonacci() {
  be::Bencode bEncode;
  if (!std::filesystem::exists(bEncodeFibonacciFile())) {
    // If Bencode file does not exist create intial sequence
    bEncode.decode(be::BufferSource{"li0ei1ee"});
  } else {
    // Parse in current sequence
    bEncode.decode(be::FileSource{bEncodeFibonacciFile()});
    // Get index of last element
    auto last = be::BRef<be::List>(bEncode.root()).size() - 1;
    // Next is sum of last two entries
    auto next = be::BRef<be::Integer>(bEncode[last]).value();
    next += be::BRef<be::Integer>(bEncode[last - 1]).value();
    // Expand array by one and add next in sequence
    bEncode[last + 1] = next;
  }
  // Write updated sequence back to file
  bEncode.encode(be::FileDestination{bEncodeFibonacciFile()});
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "Bencode_Fibonacci.log");
    PLOG_INFO << "Bencode_Fibonacci started ...";
    // Log version
    PLOG_INFO << be::Bencode().version();
    // Update current sequence
    nextFibonacci();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Fibonacci exited.";
  exit(EXIT_SUCCESS);
}