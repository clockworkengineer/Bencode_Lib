#pragma once
// ============
// STL includes
// ============
#include <cstdlib>
#include <iostream>
#include <filesystem>
struct TorrentFileDetails
{
    std::string path;
    std::uint64_t length{};
};
struct TorrentMetaInfo
{
    std::string announce;
    std::vector<std::string> announceList;
    std::string attr;
    std::string comment;
    std::uint64_t creationDate{};
    std::string createdBy;
    std::vector<TorrentFileDetails> files;
    std::uint64_t length{};
    std::string name;
    std::uint64_t pieceLength{};
    std::string pieces;
    std::uint64_t privateBitMask{};
    std::string source;
    std::vector<std::string> urlList;
};