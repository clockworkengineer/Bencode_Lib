#include "Bencode.hpp"
#include "Bencode_Core.hpp"

#include <chrono>
#include <cstdlib>
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

static void printUsage(const char *programName) {
  std::cout
      << "Usage: " << programName
      << " [count] [value-size] [iterations] [mode]\n"
      << "  count: number of dictionary entries (default 5000)\n"
      << "  value-size: bytes per value string (default 128)\n"
      << "  iterations: number of parse/stringify iterations (default 5)\n"
      << "  mode: roundtrip|parse|stringify (default roundtrip)\n";
}

static bool parseArg(const char *arg, size_t &output) {
  try {
    const auto value = std::stoull(arg);
    output = static_cast<size_t>(value);
    return true;
  } catch (...) {
    return false;
  }
}

enum class BenchmarkMode { Roundtrip, ParseOnly, StringifyOnly };

static bool parseModeArg(const char *arg, BenchmarkMode &mode) {
  const std::string value(arg);
  if (value == "roundtrip") {
    mode = BenchmarkMode::Roundtrip;
    return true;
  }
  if (value == "parse") {
    mode = BenchmarkMode::ParseOnly;
    return true;
  }
  if (value == "stringify") {
    mode = BenchmarkMode::StringifyOnly;
    return true;
  }
  return false;
}

static std::string modeName(BenchmarkMode mode) {
  switch (mode) {
  case BenchmarkMode::ParseOnly:
    return "parse-only";
  case BenchmarkMode::StringifyOnly:
    return "stringify-only";
  case BenchmarkMode::Roundtrip:
  default:
    return "roundtrip";
  }
}

static constexpr double kBaselineParseMBs = 6.31829;
static constexpr double kBaselineStringifyMBs = 13.241;

int main(int argc, char *argv[]) {
#if BENCODE_ENABLE_DYNAMIC_ALLOCATION
  size_t dictionarySize = 5000;
#else
  size_t dictionarySize = BENCODE_MAX_CONTAINER_SIZE;
#endif
  size_t valueSize = 128;
  int iterations = 5;

  if (argc > 1) {
    if (!parseArg(argv[1], dictionarySize) || dictionarySize == 0) {
      printUsage(argv[0]);
      return 1;
    }
  }
  if (argc > 2) {
    if (!parseArg(argv[2], valueSize) || valueSize == 0) {
      printUsage(argv[0]);
      return 1;
    }
  }
  BenchmarkMode mode = BenchmarkMode::Roundtrip;
  if (argc > 3) {
    size_t iterationCount = 0;
    if (!parseArg(argv[3], iterationCount) || iterationCount == 0) {
      printUsage(argv[0]);
      return 1;
    }
    iterations = static_cast<int>(iterationCount);
  }
  if (argc > 4) {
    if (!parseModeArg(argv[4], mode)) {
      printUsage(argv[0]);
      return 1;
    }
  }

  const std::string encoded = makeLargeBencode(dictionarySize, valueSize);
  const double dataMB = static_cast<double>(encoded.size()) / (1024.0 * 1024.0);

  std::cout << "Benchmark mode: " << modeName(mode) << "\n";

  ObjectPool<List>::resetStats();
  ObjectPool<Dictionary>::resetStats();

  std::cout << "Benchmark: parsing and stringifying " << dictionarySize
            << " dictionary entries (" << encoded.size() << " bytes)\n";

  auto runParseBenchmark = [&](int iterations) {
    double seconds = 0.0;
    for (int i = 0; i < iterations; ++i) {
#if BENCODE_ENABLE_EXCEPTIONS
      try {
        BufferSource source(encoded);
        const auto start = high_resolution_clock::now();
        Bencode bencode;
        bencode.parse(source);
        const auto end = high_resolution_clock::now();
        const double iterationTime = duration<double>(end - start).count();
        seconds += iterationTime;
        std::cout << "Parse iteration " << (i + 1) << ": " << iterationTime
                  << " s\n";
      } catch (const std::exception &ex) {
        std::cerr << "Parse iteration " << (i + 1) << " failed: " << ex.what()
                  << "\n";
        std::exit(1);
      }
#else
      BufferSource source(encoded);
      const auto start = high_resolution_clock::now();
      Bencode bencode;
      ParseStatus status = bencode.parse(source);
      const auto end = high_resolution_clock::now();
      const double iterationTime = duration<double>(end - start).count();
      if (!status.ok()) {
        std::cerr << "Parse iteration " << (i + 1)
                  << " failed: " << status.message << "\n";
        std::exit(1);
      }
      seconds += iterationTime;
      std::cout << "Parse iteration " << (i + 1) << ": " << iterationTime
                << " s\n";
#endif
    }
    return seconds;
  };

  auto runStringifyBenchmark = [&](int iterations) {
    double seconds = 0.0;
    for (int i = 0; i < iterations; ++i) {
      try {
        BufferSource source(encoded);
        Bencode bencode;
#if BENCODE_ENABLE_EXCEPTIONS
        bencode.parse(source);
#else
        ParseStatus status = bencode.parse(source);
        if (!status.ok()) {
          std::cerr << "Stringify parse iteration " << (i + 1)
                    << " failed: " << status.message << "\n";
          std::exit(1);
        }
#endif
        BufferDestination destination;
        const auto start = high_resolution_clock::now();
        bencode.stringify(destination);
        const auto end = high_resolution_clock::now();
        const double iterationTime = duration<double>(end - start).count();
        seconds += iterationTime;
        std::cout << "Stringify iteration " << (i + 1) << ": " << iterationTime
                  << " s\n";
        if (destination.toString().size() != encoded.size()) {
          std::cerr << "Stringify output size mismatch\n";
          std::exit(1);
        }
      } catch (const std::exception &ex) {
        std::cerr << "Stringify iteration " << (i + 1)
                  << " failed: " << ex.what() << "\n";
        std::exit(1);
      }
    }
    return seconds;
  };

  auto runRoundtripBenchmark = [&](int iterations) {
    double parseSeconds = 0.0;
    double stringifySeconds = 0.0;
    for (int i = 0; i < iterations; ++i) {
#if BENCODE_ENABLE_EXCEPTIONS
      try {
        BufferSource source(encoded);
        const auto parseStart = high_resolution_clock::now();
        Bencode bencode;
        bencode.parse(source);
        const auto parseEnd = high_resolution_clock::now();
        const double parseTime =
            duration<double>(parseEnd - parseStart).count();
        parseSeconds += parseTime;
        std::cout << "Parse iteration " << (i + 1) << ": " << parseTime
                  << " s\n";
        BufferDestination destination;
        const auto stringifyStart = high_resolution_clock::now();
        bencode.stringify(destination);
        const auto stringifyEnd = high_resolution_clock::now();
        const double stringifyTime =
            duration<double>(stringifyEnd - stringifyStart).count();
        stringifySeconds += stringifyTime;
        std::cout << "Stringify iteration " << (i + 1) << ": " << stringifyTime
                  << " s\n";
        if (destination.toString().size() != encoded.size()) {
          std::cerr << "Stringify output size mismatch\n";
          std::exit(1);
        }
      } catch (const std::exception &ex) {
        std::cerr << "Roundtrip iteration " << (i + 1)
                  << " failed: " << ex.what() << "\n";
        std::exit(1);
      }
#else
      BufferSource source(encoded);
      const auto parseStart = high_resolution_clock::now();
      Bencode bencode;
      ParseStatus status = bencode.parse(source);
      const auto parseEnd = high_resolution_clock::now();
      const double parseTime = duration<double>(parseEnd - parseStart).count();
      if (!status.ok()) {
        std::cerr << "Roundtrip parse iteration " << (i + 1)
                  << " failed: " << status.message << "\n";
        std::exit(1);
      }
      parseSeconds += parseTime;
      std::cout << "Parse iteration " << (i + 1) << ": " << parseTime << " s\n";
      BufferDestination destination;
      const auto stringifyStart = high_resolution_clock::now();
      bencode.stringify(destination);
      const auto stringifyEnd = high_resolution_clock::now();
      const double stringifyTime =
          duration<double>(stringifyEnd - stringifyStart).count();
      stringifySeconds += stringifyTime;
      std::cout << "Stringify iteration " << (i + 1) << ": " << stringifyTime
                << " s\n";
      if (destination.toString().size() != encoded.size()) {
        std::cerr << "Stringify output size mismatch\n";
        std::exit(1);
      }
#endif
    }
    return std::pair<double, double>{parseSeconds, stringifySeconds};
  };

  double parseSeconds = 0.0;
  double stringifySeconds = 0.0;
  if (mode == BenchmarkMode::ParseOnly) {
    parseSeconds = runParseBenchmark(iterations);
  } else if (mode == BenchmarkMode::StringifyOnly) {
    stringifySeconds = runStringifyBenchmark(iterations);
  } else {
    auto result = runRoundtripBenchmark(iterations);
    parseSeconds = result.first;
    stringifySeconds = result.second;
  }

  const double parseThroughput =
      parseSeconds > 0.0 ? dataMB * iterations / parseSeconds : 0.0;
  const double stringifyThroughput =
      stringifySeconds > 0.0 ? dataMB * iterations / stringifySeconds : 0.0;

  std::cout << "Parse time: " << parseSeconds << " s"
            << " (" << parseThroughput << " MB/s)\n";
  std::cout << "Stringify time: " << stringifySeconds << " s"
            << " (" << stringifyThroughput << " MB/s)\n";
  std::cout << "Average round-trip size: " << encoded.size() << " bytes\n";

  const auto listStats = ObjectPool<List>::getStats();
  const auto dictStats = ObjectPool<Dictionary>::getStats();
  std::cout << "List pool: acquired=" << listStats.acquired
            << ", created=" << listStats.created
            << ", reused=" << listStats.reused
            << ", released=" << listStats.released << "\n";
  std::cout << "Dictionary pool: acquired=" << dictStats.acquired
            << ", created=" << dictStats.created
            << ", reused=" << dictStats.reused
            << ", released=" << dictStats.released << "\n";

  std::cout << "Baseline parse throughput: " << kBaselineParseMBs << " MB/s\n";
  std::cout << "Baseline stringify throughput: " << kBaselineStringifyMBs
            << " MB/s\n";
  std::cout << "Parse improvement: "
            << ((parseThroughput / kBaselineParseMBs - 1.0) * 100.0) << " %\n";
  std::cout << "Stringify improvement: "
            << ((stringifyThroughput / kBaselineStringifyMBs - 1.0) * 100.0)
            << " %\n";

  return 0;
}
