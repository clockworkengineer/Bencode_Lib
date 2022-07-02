#pragma once
// ============
// STL includes
// ============
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <string>
//
// Torrent file details
//
struct TorrentFileDetails
{
    TorrentFileDetails(const std::string &path, std::uint64_t length) : path(path), length(length) 
    {
    }
    std::string path;           // Full file path name
    std::uint64_t length{};     // File length in bytes
};
//
// Torrent file meta information
//
struct TorrentMetaInfo
{
    std::string announce;                       // Main tracker server
    std::vector<std::string> announceList;      // Backup tracker server list
    std::string encoding;                       // Character encoding used for strings (optional)
    std::string attribute;                      // Default file attributes
    std::string comment;                        // Comment
    std::uint64_t creationDate{};               // Creation date
    std::string createdBy;                      // Created by (author)
    std::vector<TorrentFileDetails> files;      // List of torrent files to download
    std::uint64_t length{};                     // Length of file to download
    std::string name;                           // Name of single file to download (root directory in multiple files)
    std::uint64_t pieceLength{};                // Piece length
    std::string pieces;                         // Number of pieces
    std::uint64_t privateBitMask{};             // Private bitmask
    std::string source;                         // Source of torrent
};