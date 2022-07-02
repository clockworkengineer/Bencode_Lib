//
// Program: Read_Torrent_Files
//
// Description: Use BencodeLib to read in torrent file details.
// Details on the torrent file format can be found at the wiki page
// https://en.wikipedia.org/wiki/Torrent_file.
//
// Dependencies: C20++, BencodeLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <stdexcept>
// =======
// Bencode
// =======
#include "Bencode.hpp"
#include "Bencode_Types.hpp"
#include "Bencode_Sources.hpp"
#include "Bencode_Destinations.hpp"
//
// Torrent includes
//
#include "TorrentInfo.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ======================
// LOCAL TYPES/DEFINITIONS
// ======================
/// <summary>
/// Get string value with named field from a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <param name="field">Field name of string value</param>
/// <returns>String value of field.</returns>
std::string getDictionaryString(const BNodeDict &bNodeDict, const char *field)
{
    if (bNodeDict.contains(field))
    {
        return (BNodeRef<BNodeString>(bNodeDict[field]).string());
    }
    return ("");
}
/// <summary>
/// Get integer value with named field from a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <param name="field">Field name of integer value</param>
/// <returns>Integer value of field.</returns>
std::uint64_t getDictionaryInteger(const BNodeDict &bNodeDict, const char *field)
{
    if (bNodeDict.contains(field))
    {
        return (BNodeRef<BNodeInteger>(bNodeDict[field]).integer());
    }
    return (0);
}
/// <summary>
///  Get vector of announce servers from a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <returns>Vector of announce server names.</returns>
std::vector<std::string> getAnnounceList(const BNodeDict &bNodeDict)
{
    // This is meant to be a simple list of strings but for some reason each string
    // is encased in its own list for an extra level (bug ?).
    if (bNodeDict.contains("announce-list"))
    {
        std::vector<std::string> servers;
        for (auto &bNode : BNodeRef<BNodeList>(bNodeDict["announce-list"]).list())
        {
            for (auto &bNodeString : BNodeRef<BNodeList>(*bNode).list())
            {
                servers.push_back(BNodeRef<BNodeString>(*bNodeString).string());
            }
        }
        return (servers);
    }
    return(std::vector<std::string>{});
}
/// <summary>
/// Construct a file path from a list of strings contained in a dictionary.
/// </summary>
/// <param name="bNodeDict">Dictionary</param>
/// <returns>Full file path name.</returns>
std::string getFilePath(const BNodeDict &bNodeDict)
{
    if (bNodeDict.contains("path"))
    {
        std::filesystem::path path{};
        for (auto &folder : BNodeRef<BNodeList>(bNodeDict["path"]).list())
        {
            path /= BNodeRef<BNodeString>(*folder).string();
        }
        return (path.string());
    }
    return("");
}
/// <summary>
/// Extract and return a vector of file details from a dictionary.
/// </summary>
/// <param name="bNodeInfoDict">Dictionary</param>
/// <returns>Vector of torrent file details.</returns>
std::vector<TorrentFileDetails> getFilesList(const BNodeDict &bNodeInfoDict)
{
    if (bNodeInfoDict.contains("files"))
    {
        std::vector<TorrentFileDetails> files;
        for (auto &file : BNodeRef<BNodeList>(bNodeInfoDict["files"]).list())
        {
            files.emplace_back(getFilePath(BNodeRef<BNodeDict>(*file)), getDictionaryInteger(BNodeRef<BNodeDict>(*file), "length"));
        }
        return (files);
    }
    return(std::vector<TorrentFileDetails>{});
}
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Load torrent file meta information into a structure for processing.
/// </summary>
/// <param name="bNode">Root BNode of decoded torrent file.</param>
/// <returns>Torrent file meta information structure.</returns>
TorrentMetaInfo getTorrentInfo(const BNode &bNode)
{
    TorrentMetaInfo info;
    if (bNode.getNodeType() != BNodeType::dictionary)
    {
        throw BencodeLib::Error("Valid torrent file not found.");
    }
    auto &bNodeTopLevelDict = BNodeRef<BNodeDict>(bNode);
    info.announce = getDictionaryString(bNodeTopLevelDict, "announce");
    info.announceList = getAnnounceList(bNodeTopLevelDict);
    info.encoding = getDictionaryString(bNodeTopLevelDict, "encoding");
    info.comment = getDictionaryString(bNodeTopLevelDict, "comment");
    info.creationDate = getDictionaryInteger(bNodeTopLevelDict, "creation date");
    info.createdBy = getDictionaryString(bNodeTopLevelDict, "created by");
    if (bNodeTopLevelDict.contains("info"))
    {
        auto &bNodeInfoDict = BNodeRef<BNodeDict>(bNodeTopLevelDict["info"]);
        info.attribute = getDictionaryString(bNodeInfoDict, "attr");
        info.length = getDictionaryInteger(bNodeInfoDict, "length");
        info.name = getDictionaryString(bNodeInfoDict, "name");
        info.pieceLength = getDictionaryInteger(bNodeInfoDict, "piece length");
        info.pieces = getDictionaryString(bNodeInfoDict, "pieces");
        info.privateBitMask = getDictionaryInteger(bNodeInfoDict, "private");
        info.source = getDictionaryString(bNodeInfoDict, "source");
        info.files = getFilesList(bNodeInfoDict);
    }
    return (info);
}
/// <summary>
/// Display to stdout meta information for torrent file.
/// </summary>
/// <param name="fileName">Torrent file name</param>
/// <param name="info">Meta information</param>
void displayTorrentInfo(const std::string &fileName, const TorrentMetaInfo &info)
{
    std::cout << "------------------------------------------------------------\n";
    std::cout << "FILE [ " << fileName << " ]\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "announce [" << info.announce << "]\n";
    std::cout << "attribute [" << info.attribute << "]\n";
    std::cout << "encoding [" << info.encoding << "]\n";
    std::cout << "comment [" << info.comment << "]\n";
    std::cout << "creation_date [" << info.creationDate << "]\n";
    std::cout << "created_by [" << info.createdBy << "]\n";
    std::cout << "length [" << info.length << "]\n";
    std::cout << "name [" << info.name << "]\n";
    std::cout << "piece length [" << info.pieceLength << "]\n";
    std::cout << "private [" << info.privateBitMask << "]\n";
    std::cout << "source [" << info.source << "]\n";
    for (const auto &file : info.files)
    {
        std::cout << "path [ " << file.path << "] length [" << file.length << "]\n";
    }
    for (const auto &announceURL : info.announceList)
    {
        std::cout << "announce url [ " << announceURL << "]\n";
    }
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    try
    {
        const std::vector<std::string> fileList{"./testData/file01.torrent",
                                                "./testData/file02.torrent",
                                                "./testData/file03.torrent",
                                                "./testData/file04.torrent",
                                                "./testData/file05.torrent"};
        Bencode bEncode;
        //
        // For each torrent file extract its information and display
        //
        for (const auto &fileName : fileList)
        {
            bEncode.decode(FileSource{fileName});
            displayTorrentInfo(fileName, getTorrentInfo(bEncode.root()));
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
    }
    exit(EXIT_SUCCESS);
}