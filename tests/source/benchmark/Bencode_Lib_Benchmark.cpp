#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include <chrono>
#include <iostream>
#include <limits>
#include <string>

using namespace Bencode_Lib;
using namespace std::chrono;

static std::string makeLargeBencode(size_t count, size_t valueSize) {
  std::string encoded = "d";
  encoded.reserve(count * (10 + valueSize));
  const auto width = std::to_string(count - 1).size();
  for (size_t index = 0; index < count; ++index) {
    std::string indexString = std::to_string(index);
    if (indexString.size() < width) {
      indexString = std::string(width - indexString.size(), '0') + indexString;
    }
    std::string key = "k" + indexString;
    std::string value(valueSize, 'x');
    encoded += std::to_string(key.size());
    encoded += ':';
    encoded += key;
    encoded += std::to_string(value.size());
    encoded += ':';
    encoded += value;
  }
  encoded += 'e';
  return encoded;
}

int main() {
  const size_t dictionarySize = 5000;
  const size_t valueSize = 128;
  const int iterations = 5;

  const std::string encoded = makeLargeBencode(dictionarySize, valueSize);
  const double dataMB = static_cast<double>(encoded.size()) / (1024.0 * 1024.0);

  std::cout << "Benchmark: parsing and stringifying " << dictionarySize
            << " dictionary entries (" << encoded.size() << " bytes)\n";

  double parseSeconds = 0.0;
  for (int i = 0; i < iterations; ++i) {
    try {
      BufferSource source(encoded);
      const auto start = high_resolution_clock::now();
      Bencode bencode;
      bencode.parse(source);
      const auto end = high_resolution_clock::now();
      const double iterationTime = duration<double>(end - start).count();
      parseSeconds += iterationTime;
      std::cout << "Parse iteration " << (i + 1) << ": " << iterationTime << " s\n";
    } catch (const std::exception &ex) {
      std::cerr << "Parse iteration " << (i + 1)
                << " failed: " << ex.what() << "\n";
      return 1;
    }
  }

  double stringifySeconds = 0.0;
  for (int i = 0; i < iterations; ++i) {
    try {
      BufferSource source(encoded);
      Bencode bencode;
      bencode.parse(source);
      BufferDestination destination;
      const auto start = high_resolution_clock::now();
      bencode.stringify(destination);
      const auto end = high_resolution_clock::now();
      const double iterationTime = duration<double>(end - start).count();
      stringifySeconds += iterationTime;
      std::cout << "Stringify iteration " << (i + 1) << ": " << iterationTime << " s\n";
      if (destination.toString().size() != encoded.size()) {
        std::cerr << "Stringify output size mismatch\n";
        return 1;
      }
    } catch (const std::exception &ex) {
      std::cerr << "Stringify iteration " << (i + 1)
                << " failed: " << ex.what() << "\n";
      return 1;
    }
  }

  const double parseThroughput = dataMB * iterations / parseSeconds;
  const double stringifyThroughput = dataMB * iterations / stringifySeconds;

  std::cout << "Parse time: " << parseSeconds << " s"
            << " (" << parseThroughput << " MB/s)\n";
  std::cout << "Stringify time: " << stringifySeconds << " s"
            << " (" << stringifyThroughput << " MB/s)\n";
  std::cout << "Average round-trip size: " << encoded.size() << " bytes\n";

  return 0;
}
