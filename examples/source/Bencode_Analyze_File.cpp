//
// Program: Bencode_Analyze_File
//
// Description: For a each Bencode file (torrent) in a directory parse it, analyze its
// Bencode tree and produce an output report of the analysis.
//
// Dependencies: C++20, Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "Bencode_Analyzer.hpp"
#include <iostream>

namespace be = Bencode_Lib;

/// <summary>
/// Parse Bencode file and analyze its Bencode tree.
/// </summary>
/// <param name="fileName">Bencode file name</param>
void processBencodeFile(const std::string_view &fileName)
{
  std::cout << "Analyzing " << fileName;
  const be::Bencode bStringify;
  Bencode_Analyzer bStringifyAnalyzer;
  bStringify.parse(be::FileSource{ fileName });
  bStringify.traverse(bStringifyAnalyzer);
  std::cout << bStringifyAnalyzer.dump();
  std::cout << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  std::cout << "Bencode_Analyze_File started ...";
  // Output Bencode Lib version and data structure metrics
  std::cout << be::Bencode::version();
  std::cout << Bencode_Analyzer::dumpNumericSizes();
  std::cout << Bencode_Analyzer::dumpNodeSizes();
  // Analyze Bencode files.
  for (auto &fileName : Utility::createTorrentFileList()) {
    try {
      processBencodeFile(fileName);
    } catch (std::exception &ex) {
      std::cerr << "Error: " << ex.what();
    }
  }
  std::cout << "Bencode_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}
