//
// Program: Bencode_Analyze_File
//
// Description: For a each Bencode file (torrent) in a directory parse it, analyze its
// Bencode tree and produce an output report of the analysis.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"
#include "Bencode_Analyzer.hpp"

namespace be = Bencode_Lib;

/// <summary>
/// Parse Bencode file and analyze its Bencode tree.
/// </summary>
/// <param name="fileName">Bencode file name</param>
void processBencodeFile(const std::string_view &fileName)
{
  PLOG_INFO << "Analyzing " << fileName;
  const be::Bencode bStringify;
  Bencode_Analyzer bStringifyAnalyzer;
  bStringify.parse(be::FileSource{ fileName });
  bStringify.traverse(bStringifyAnalyzer);
  PLOG_INFO << bStringifyAnalyzer.dump();
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  init(plog::debug, "Bencode_Analyze_File.log");
  PLOG_INFO << "Bencode_Analyze_File started ...";
  // Output Bencode Lib version and data structure metrics
  PLOG_INFO << be::Bencode::version();
  PLOG_INFO << Bencode_Analyzer::dumpNumericSizes();
  PLOG_INFO << Bencode_Analyzer::dumpNodeSizes();
  // Analyze Bencode files.
  for (auto &fileName : Utility::createTorrentFileList()) {
    try {
      processBencodeFile(fileName);
    } catch (std::exception &ex) {
      PLOG_ERROR << "Error: " << ex.what();
    }
  }
  PLOG_INFO << "Bencode_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}
