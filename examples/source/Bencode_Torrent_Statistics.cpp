//
// Program: Bencode_Torrent_Statistics
//
// Description: Parse all torrent files in the files directory and aggregate
// statistics: total file count, total download size, unique tracker URLs, and
// a breakdown of file extensions found across all torrents.
//
// Dependencies: C++20, PLOG, Bencode_Lib.
//

#include "Bencode_Utility.hpp"

namespace be = Bencode_Lib;

struct TorrentStats {
  std::uint64_t totalFiles{};
  std::uint64_t totalBytes{};
  std::set<std::string> trackers;
  std::map<std::string, std::uint64_t> extensionCounts;
};

/// <summary>
/// Extract the file extension from a path string, lower-cased.
/// Returns "(none)" if there is no extension.
/// </summary>
std::string fileExtension(const std::string &path) {
  const std::filesystem::path p{path};
  const auto ext = p.extension().string();
  if (ext.empty()) return "(none)";
  std::string lower = ext;
  std::transform(lower.begin(), lower.end(), lower.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return lower;
}

/// <summary>
/// Accumulate statistics from a single torrent's parsed Bencode tree.
/// </summary>
void accumulateStats(const be::Bencode &torrent, TorrentStats &stats) {
  auto &root = be::NRef<be::Dictionary>(torrent.root());

  // Collect tracker URL
  if (root.contains("announce")) {
    stats.trackers.insert(
        std::string(be::NRef<be::String>(root["announce"]).value()));
  }

  if (!root.contains("info")) {
    return;
  }

  auto &info = be::NRef<be::Dictionary>(root["info"]);

  // Multi-file torrent
  if (info.contains("files")) {
    for (auto &fileNode : be::NRef<be::List>(info["files"]).value()) {
      auto &fileDict = be::NRef<be::Dictionary>(fileNode);
      const auto length = be::NRef<be::Integer>(fileDict["length"]).value();
      stats.totalBytes += static_cast<std::uint64_t>(length);
      stats.totalFiles++;

      // Build path from "path" list
      if (fileDict.contains("path")) {
        std::filesystem::path filePath;
        for (auto &part : be::NRef<be::List>(fileDict["path"]).value()) {
          filePath /= be::NRef<be::String>(part).value();
        }
        stats.extensionCounts[fileExtension(filePath.string())]++;
      }
    }
  } else if (info.contains("length")) {
    // Single-file torrent
    const auto length = be::NRef<be::Integer>(info["length"]).value();
    stats.totalBytes += static_cast<std::uint64_t>(length);
    stats.totalFiles++;

    if (info.contains("name")) {
      const std::string name{be::NRef<be::String>(info["name"]).value()};
      stats.extensionCounts[fileExtension(name)]++;
    }
  }
}

/// <summary>
/// Format a byte count as a human-readable string (B / KB / MB / GB).
/// </summary>
std::string formatBytes(std::uint64_t bytes) {
  constexpr std::uint64_t KB = 1024;
  constexpr std::uint64_t MB = 1024 * KB;
  constexpr std::uint64_t GB = 1024 * MB;
  std::ostringstream oss;
  if (bytes >= GB)      oss << (bytes / GB) << " GB";
  else if (bytes >= MB) oss << (bytes / MB) << " MB";
  else if (bytes >= KB) oss << (bytes / KB) << " KB";
  else                  oss << bytes << " B";
  return oss.str();
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    init(plog::debug, "Bencode_Torrent_Statistics.log");
    PLOG_INFO << "Bencode_Torrent_Statistics started ...";
    PLOG_INFO << be::Bencode::version();

    TorrentStats stats;
    const auto fileList = Utility::createTorrentFileList();

    for (const auto &fileName : fileList) {
      try {
        be::Bencode torrent;
        torrent.parse(be::FileSource{fileName});
        accumulateStats(torrent, stats);
        PLOG_INFO << "Processed " << fileName;
      } catch (std::exception &ex) {
        PLOG_ERROR << "Failed to process " << fileName << ": " << ex.what();
      }
    }

    PLOG_INFO << "=== Torrent Statistics ===";
    PLOG_INFO << "  Torrents processed : " << fileList.size();
    PLOG_INFO << "  Total files        : " << stats.totalFiles;
    PLOG_INFO << "  Total size         : " << formatBytes(stats.totalBytes)
              << "  (" << stats.totalBytes << " bytes)";

    PLOG_INFO << "=== Unique Trackers (" << stats.trackers.size() << ") ===";
    for (const auto &tracker : stats.trackers) {
      PLOG_INFO << "  " << tracker;
    }

    PLOG_INFO << "=== File Extensions ===";
    for (const auto &[ext, count] : stats.extensionCounts) {
      PLOG_INFO << "  " << ext << " : " << count;
    }

  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "Bencode_Torrent_Statistics exited.";
  exit(EXIT_SUCCESS);
}
