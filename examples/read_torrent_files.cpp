//
// Program: read_torrent_files
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
//
// C++ STL
//
#include <cstdlib>
#include <iostream>
#include <filesystem>
//
// Bencode includes
//
#include "Bencode.hpp"
#include "BencodeSources.hpp"
#include "BencodeDestinations.hpp"
//
// Torrent includes
//
#include "TorrentInfo.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ======================
// LOCAL TYES/DEFINITIONS
// ======================

/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeDict"></param>
/// <param name="field"></param>
/// <param name="str"></param>
/// ********************************************************************************
void getDictionaryString(BNodeDict &bNodeDict, const char *field, std::string &str)
{
    if (bNodeDict.containsKey(field))
    {
        str = BNodeRef<BNodeString>(bNodeDict[field]).getString();
    }
}
/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeDict"></param>
/// <param name="field"></param>
/// <param name="integer"></param>
/// ********************************************************************************
void getDictionaryInteger(BNodeDict &bNodeDict, const char *field, std::uint64_t &integer)
{
    if (bNodeDict.containsKey(field))
    {
        integer = BNodeRef<BNodeInteger>(bNodeDict[field]).getInteger();
    }
}
/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeDict"></param>
/// <param name="strings"></param>
/// ********************************************************************************
void getAnnouceList(BNodeDict &bNodeDict, std::vector<std::string> &strings)
{
    // This is meant to be a simple list of strings but for some reason each string
    // is encased in its own list for an extra level (bug ?).
    if (bNodeDict.containsKey("announce-list"))
    {
        auto &bNodeList = BNodeRef<BNodeList>(bNodeDict["announce-list"]);
        for (auto &bNode : bNodeList.getList())
        {
            auto &bNodeInnerList = BNodeRef<BNodeList>(*bNode);
            for (auto &bNodeString : bNodeInnerList.getList())
            {
                strings.push_back(BNodeRef<BNodeString>(*bNodeString).getString());
            }
        }
    }
}
/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeDict"></param>
/// <param name="filePath"></param>
/// ********************************************************************************
void getFilePath(BNodeDict &bNodeDict, std::string &filePath)
{
    if (bNodeDict.containsKey("path"))
    {
        std::filesystem::path path{};
        for (auto &folder : BNodeRef<BNodeList>(bNodeDict["path"]).getList())
        {
            path /= BNodeRef<BNodeString>(*folder).getString();
        }
        filePath = path.string();
    }
}
/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeInfoDict"></param>
/// <param name="files"></param>
/// ********************************************************************************
void getFilesList(BNodeDict &bNodeInfoDict, std::vector<TorrentFileDetails> &files)
{
    if (bNodeInfoDict.containsKey("files"))
    {
        for (auto &file : BNodeRef<BNodeList>(bNodeInfoDict["files"]).getList())
        {
            TorrentFileDetails fileEntry;
            getDictionaryInteger(BNodeRef<BNodeDict>(*file), "length", fileEntry.length);
            getFilePath(BNodeRef<BNodeDict>(*file), fileEntry.path);
            files.push_back(fileEntry);
        }
    }
}
/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNodeDict"></param>
/// <param name="urlList"></param>
/// ********************************************************************************
void getURLList(BNodeDict &bNodeDict, std::vector<std::string> &urlList)
{
    if (bNodeDict.containsKey("url-list"))
    {
        if (bNodeDict["url-list"].nodeType == BNodeType::string)
        {
            urlList.push_back(BNodeRef<BNodeString>(bNodeDict["url-list"]).getString());
        }
        else if (bNodeDict["url-list"].nodeType == BNodeType::list)
        {
            for (auto &bNodeURLString : BNodeRef<BNodeList>(bNodeDict["url-list"]).getList())
            {
                urlList.push_back(BNodeRef<BNodeString>(*bNodeURLString).getString());
            }
        }
    }
}
// ===============
// LOCAL FUNCTIONS
// ===============

/// ********************************************************************************
/// <summary>
///
/// </summary>
/// <param name="bNode"></param>
/// <returns></returns>
/// ********************************************************************************
TorrentMetaInfo getTorrentInfo(BNode &bNode)
{
    TorrentMetaInfo info;
    if (bNode.nodeType != BNodeType::dictionary)
    {
        throw std::exception("Valid torrent file not found.");
    }
    auto &bNodeTopLevelDict = BNodeRef<BNodeDict>(bNode);
    getDictionaryString(bNodeTopLevelDict, "announce", info.announce);
    getAnnouceList(bNodeTopLevelDict, info.annouceList);
    getDictionaryString(bNodeTopLevelDict, "comment", info.comment);
    getDictionaryInteger(bNodeTopLevelDict, "creation date", info.creationDate);
    getDictionaryString(bNodeTopLevelDict, "created by", info.createdBy);
    if (bNodeTopLevelDict.containsKey("info"))
    {
        auto &bNodeInfoDict = BNodeRef<BNodeDict>(bNodeTopLevelDict["info"]);
        getDictionaryString(bNodeInfoDict, "attr", info.attr);
        getDictionaryInteger(bNodeInfoDict, "length", info.length);
        getDictionaryString(bNodeInfoDict, "name", info.name);
        getDictionaryInteger(bNodeInfoDict, "piece length", info.pieceLength);
        getDictionaryString(bNodeInfoDict, "pieces", info.pieces);
        getDictionaryInteger(bNodeInfoDict, "private", info.privateBitMask);
        getDictionaryString(bNodeInfoDict, "source", info.source);
        getFilesList(bNodeInfoDict, info.files);
    }
    getURLList(bNodeTopLevelDict, info.urlList);
    return (info);
}
/// ********************************************************************************
/// <summary>
/// 
/// </summary>
/// <param name="fileName"></param>
/// <param name="info"></param>
/// ********************************************************************************
void displayTorrentInfo(const std::string &fileName, const TorrentMetaInfo &info)
{
    std::cout << "------------------------------------------------------------\n";
    std::cout << "FILE [ " << fileName << " ]\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "announce [" << info.announce << "]\n";
    std::cout << "attr [" << info.attr << "]\n";
    std::cout << "comment [" << info.comment << "]\n";
    std::cout << "creation_date [" << info.creationDate << "]\n";
    std::cout << "created_by [" << info.createdBy << "]\n";
    std::cout << "length [" << info.length << "]\n";
    std::cout << "name [" << info.name << "]\n";
    std::cout << "piece length [" << info.pieceLength << "]\n";
    // std::cout << "pieces [" << info.pieces << "]\n";
    std::cout << "private [" << info.privateBitMask << "]\n";
    std::cout << "source [" << info.source << "]\n";
    for (const auto &file : info.files)
    {
        std::cout << "path [ " << file.path << "] length [" << file.length << "]\n";
    }
    for (const auto &announceURL : info.annouceList)
    {
        std::cout << "announce url [ " << announceURL << "]\n";
    }
    for (const auto &url : info.urlList)
    {
        std::cout << "url [ " << url << "]\n";
    }
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    try
    {
        std::vector<std::string> fileList{"./testData/file01.torrent",
                                          "./testData/file02.torrent",
                                          "./testData/file03.torrent",
                                          "./testData/file04.torrent",
                                          "./testData/file05.torrent"};
        Bencode bEncode;
        TorrentMetaInfo info;

        for (auto &fileName : fileList)
        {
            bEncode.decode(FileSource{fileName});
            info = getTorrentInfo(*bEncode);
            displayTorrentInfo(fileName, info);
        }
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }
    exit(EXIT_SUCCESS);
}