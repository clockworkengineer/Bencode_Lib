//
// Program: read_torrent_file
//
// Description: Use BencodeLib to read in torrent file details.
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
void getDictionaryString(BNodeDict &bNodeDict, const char *field, std::string &str)
{
    if (bNodeDict.containsKey(field))
    {
        str = BNodeRef<BNodeString>(bNodeDict[field]).getString();
    }
}
void getDictionaryInteger(BNodeDict &bNodeDict, const char *field, std::uint64_t &integer)
{
    if (bNodeDict.containsKey(field))
    {
        integer = BNodeRef<BNodeInteger>(bNodeDict[field]).getInteger();
    }
}
void getAnnouceList(BNodeDict &bNodeDict, std::vector<std::string> &strings)
{
    // This is meant to be a simple list of strings but for some reason each string
    // is encased in its own list for an extra level (bug ?).
    if (bNodeDict.containsKey("announce-list"))
    {
        BNodeList &bNodeList = BNodeRef<BNodeList>(bNodeDict["announce-list"]);
        for (auto &bNode : bNodeList.getList())
        {
            BNodeList &bNodeInnerList = BNodeRef<BNodeList>(*bNode);
            for (auto &bNodeString : bNodeInnerList.getList())
            {
                strings.push_back(BNodeRef<BNodeString>(*bNodeString).getString());
            }
        }
    }
}
void getFilesList(BNodeDict &bNodeInfoDict, std::vector<TorrentFileDetails> &files)
{

    if (bNodeInfoDict.containsKey("files"))
    {
        BNodeList &bNodeFilesList = BNodeRef<BNodeList>(bNodeInfoDict["files"]);
        for (auto &file : bNodeFilesList.getList())
        {
            BNodeDict &bNodeFileDict = BNodeRef<BNodeDict>(*file);
            TorrentFileDetails fileEntry;
            if (bNodeFileDict.containsKey("length"))
            {
                fileEntry.length = BNodeRef<BNodeInteger>(bNodeFileDict["length"]).getInteger();
            }
            if (bNodeFileDict.containsKey("path"))
            {
                BNodeList &bNodePathList = BNodeRef<BNodeList>(bNodeFileDict["path"]);
                std::filesystem::path path{};
                for (auto &folder : bNodePathList.getList())
                {
                    path += "/" + BNodeRef<BNodeString>(*folder).getString();
                }
                fileEntry.path = path.string();
            }
            files.push_back(fileEntry);
        }
    }
}
// ===============
// LOCAL FUNCTIONS
// ===============
TorrentMetaInfo getTorrentInfo(BNode &bNode)
{
    TorrentMetaInfo info;

    if (bNode.nodeType != BNodeType::dictionary)
    {
        throw std::exception("Valid torrent file not found.");
    }

    BNodeDict &bNodeTopLevelDict = BNodeRef<BNodeDict>(bNode);
    getDictionaryString(bNodeTopLevelDict, "announce", info.announce);
    getAnnouceList(bNodeTopLevelDict, info.annouceList);
    getDictionaryString(bNodeTopLevelDict, "comment", info.comment);
    getDictionaryInteger(bNodeTopLevelDict, "creation date", info.creationDate);
    getDictionaryString(bNodeTopLevelDict, "created by", info.createdBy);

    if (bNodeTopLevelDict.containsKey("info"))
    {
        BNodeDict &bNodeInfoDict = BNodeRef<BNodeDict>(bNodeTopLevelDict["info"]);
        getDictionaryString(bNodeInfoDict, "attr", info.attr);
        getDictionaryInteger(bNodeInfoDict, "length", info.length);
        getDictionaryString(bNodeInfoDict, "name", info.name);
        getDictionaryInteger(bNodeInfoDict, "piece length", info.pieceLength);
        getDictionaryString(bNodeInfoDict, "pieces", info.pieces);
        getDictionaryInteger(bNodeInfoDict, "private", info.privateBitMask);
        getDictionaryString(bNodeInfoDict, "source", info.source);
        getFilesList(bNodeInfoDict, info.files);
    }
    if (bNodeTopLevelDict.containsKey("url-list"))
    {
        if (bNodeTopLevelDict["url-list"].nodeType == BNodeType::string)
        {
            info.urlList.push_back(BNodeRef<BNodeString>(bNodeTopLevelDict["url-list"]).getString());
        }
        else if (bNodeTopLevelDict["url-list"].nodeType == BNodeType::list)
        {
            for (auto &bNodeURLString : BNodeRef<BNodeList>(bNodeTopLevelDict["url-list"]).getList())
            {
                info.urlList.push_back(BNodeRef<BNodeString>(*bNodeURLString).getString());
            }
        }
    }
    return (info);
}
void displayTorrentInfo(const TorrentMetaInfo &info)
{
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
    for (auto file : info.files)
    {
        std::cout << "path [ " << file.path << "] length [" << file.length << "]\n";
    }
    for (auto &announceURL : info.annouceList)
    {
        std::cout << "announce url [ " << announceURL << "]\n";
    }
    for (auto &url : info.urlList)
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
        Bencode bEncode;
        TorrentMetaInfo info;
        // bEncode.decode(FileSource{"./testData/file01.torrent"});
        // info = getTorrentInfo(*bEncode);
        // displayTorrentInfo(info);
        // bEncode.decode(FileSource{"./testData/file02.torrent"});
        // info = getTorrentInfo(*bEncode);
        // displayTorrentInfo(info);
        // bEncode.decode(FileSource{"./testData/file03.torrent"});
        // info = getTorrentInfo(*bEncode);
        // displayTorrentInfo(info);
        // bEncode.decode(FileSource{"./testData/file04.torrent"});
        // info = getTorrentInfo(*bEncode);
        // displayTorrentInfo(info);
        bEncode.decode(FileSource{"./testData/file05.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }
    exit(EXIT_SUCCESS);
}