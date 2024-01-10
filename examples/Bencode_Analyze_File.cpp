//
// Program: Bencode_Analyze_File
//
// Description: For a each Bencode file (torrent) in a directory parse it, then analyze its
// Bencode tree and produce a output report of the analysis.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Utility.hpp"

#include "Bencode_Analyzer.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace be = Bencode_Lib;

/// <summary>
/// Parse Bencode file and analyze its Bencode tree.
/// </summary>
/// <param name="fileName">Bencode file name</param>
void processBencodeFile(const std::string &fileName)
{
  PLOG_INFO << "Analyzing " << fileName;
  const be::Bencode bEncode;
  Bencode_Analyzer bEncodeAnalyzer;
  bEncode.decode(be::FileSource{ fileName });
  bEncode.traverse(bEncodeAnalyzer);
  PLOG_INFO << bEncodeAnalyzer.dump();
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "Bencode_Analyze_File.log");
  PLOG_INFO << "Bencode_Analyze_File started ...";
  // Output Bencode Lib version and data structure metrics
  PLOG_INFO << be::Bencode().version();
  PLOG_INFO << Bencode_Analyzer::dumpNumericSizes();
  PLOG_INFO << Bencode_Analyzer::dumpBNodeSizes();
  // Analyze Bencode files.
  for (auto &fileName : Utility::createTorrentFileList()) {
    try {
      processBencodeFile(fileName);
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "Bencode_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}
