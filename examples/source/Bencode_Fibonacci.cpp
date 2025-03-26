//
// Program: Bencode_Fibonacci
//
// Description: On each activation add the next entry in the fibonacci
// sequence to the array stored in fibonacci.ben; if the file does
// not exist then create the initial sequence of li0ei1ee.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

/// <summary>
/// Return Fibonaci bStringify file name.
/// </summary>
/// <returns>Bencode settings file name.</returns>
std::string bStringifyFibonacciFile() {
  return (std::filesystem::current_path() / "files" / "fibonacci.ben").string();
}
/// <summary>
/// Read in current fibonacci sequence from Bencode file, calculate the
///  next in sequence and write back to Bencode file.
/// </summary>
void nextFibonacci() {
  be::Bencode bStringify;
  if (!std::filesystem::exists(bStringifyFibonacciFile())) {
    // If Bencode file does not exist create intial sequence
    bStringify.parse(be::BufferSource{"li0ei1ee"});
  } else {
    // Parse in current sequence
    bStringify.parse(be::FileSource{bStringifyFibonacciFile()});
    // Get index of last element
    const auto last = be::BRef<be::List>(bStringify.root()).size() - 1;
    // Next is sum of last two entries
    auto next = be::BRef<be::Integer>(bStringify[last]).value();
    next += be::BRef<be::Integer>(bStringify[last - 1]).value();
    // Expand array by one and add next in sequence
    bStringify[last + 1] = next;
  }
  // Write updated sequence back to file
  bStringify.stringify(be::FileDestination{bStringifyFibonacciFile()});
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    init(plog::debug, "Bencode_Fibonacci.log");
    PLOG_INFO << "Bencode_Fibonacci started ...";
    // Log version
    PLOG_INFO << be::Bencode::version();
    // Update current sequence
    nextFibonacci();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Fibonacci exited.";
  exit(EXIT_SUCCESS);
}