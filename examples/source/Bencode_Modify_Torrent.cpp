//
// Program: Bencode_Modify_Torrent
//
// Description: Demonstrates round-trip modification of a Bencode file.
// Each torrent in the files directory is parsed, a "comment" field is added
// (or updated) in the root dictionary, and a modified copy is written to a
// new .ben file.  The modified file is then re-parsed to confirm the change.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

/// <summary>
/// Return a suitable output path for the modified torrent alongside originals.
/// </summary>
std::string modifiedPath(const std::string &original) {
  return Utility::createFileName(original, ".modified.ben");
}

/// <summary>
/// Parse torrent, inject/overwrite its "comment" field, and save.
/// </summary>
void injectComment(const std::string &fileName, const std::string &comment) {
  be::Bencode torrent;
  torrent.parse(be::FileSource{fileName});

  // Modify or add the top-level "comment" key
  torrent["comment"] = comment;

  const std::string outFile = modifiedPath(fileName);
  torrent.stringify(be::FileDestination{outFile});
  PLOG_INFO << "Written modified torrent to " << outFile;
}

/// <summary>
/// Re-parse the modified file and verify the comment round-trips correctly.
/// </summary>
void verifyComment(const std::string &fileName, const std::string &expected) {
  be::Bencode torrent;
  const std::string modFile = modifiedPath(fileName);
  torrent.parse(be::FileSource{modFile});

  auto &root = be::NRef<be::Dictionary>(torrent.root());
  if (root.contains("comment")) {
    const auto actual = be::NRef<be::String>(root["comment"]).value();
    if (actual == expected) {
      PLOG_INFO << "  Verified comment OK: \"" << actual << "\"";
    } else {
      PLOG_ERROR << "  Comment mismatch! Expected \"" << expected << "\" got \""
                 << actual << "\"";
    }
  } else {
    PLOG_ERROR << "  'comment' field missing after round-trip in " << modFile;
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    init(plog::debug, "Bencode_Modify_Torrent.log");
    PLOG_INFO << "Bencode_Modify_Torrent started ...";
    PLOG_INFO << be::Bencode::version();

    const std::string newComment = "Modified by Bencode_Lib example";

    for (const auto &fileName : Utility::createTorrentFileList()) {
      try {
        PLOG_INFO << "Processing " << fileName;
        injectComment(fileName, newComment);
        verifyComment(fileName, newComment);
      } catch (std::exception &ex) {
        PLOG_ERROR << "Error processing " << fileName << ": " << ex.what();
      }
    }
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Modify_Torrent exited.";
  exit(EXIT_SUCCESS);
}
