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
// =======================
// Bencode class namespace
// =======================
using namespace BencodeLib;
// ======================
// LOCAL TYES/DEFINITIONS
// ======================
struct TorrentFile
{
    std::string path;
    std::uint64_t length;
};
struct TorrentInfo
{
    std::string announce;
    std::vector<std::string> annouceList;
    std::string attr;
    std::string comment;
    std::uint64_t creationDate;
    std::string createdBy;
    std::vector<TorrentFile> files;
    std::uint64_t length;
    std::string name;
    std::uint64_t pieceLength;
    std::string pieces;
    std::uint64_t privateBitMask;
    std::string source;
    std::vector<std::string> urlList;
};
void getListString(BNodeList &nodeList, std::vector<std::string> &strings)
{
    for (auto &bNode : nodeList.getList())
    {
        BNodeList &bNodeInnerList = BNodeRef<BNodeList>(*bNode);
        for (auto &bNodeString : bNodeInnerList.getList())
        {
            strings.push_back(BNodeRef<BNodeString>(*bNodeString).getString());
        }
    }
}
// ===============
// LOCAL FUNCTIONS
// ===============
TorrentInfo getTorrentInfo(BNode &bNode)
{
    TorrentInfo info;
    if (bNode.nodeType != BNodeType::dictionary)
    {
        throw std::exception("Valid torrent file not found.");
    }
    BNodeDict &bNodeTopLevelDict = BNodeRef<BNodeDict>(bNode);
    if (bNodeTopLevelDict.containsKey("announce"))
    {
        info.announce = BNodeRef<BNodeString>(bNodeTopLevelDict["announce"]).getString();
    }
    if (bNodeTopLevelDict.containsKey("announce-list"))
    {
        getListString(BNodeRef<BNodeList>(bNodeTopLevelDict["announce-list"]), info.annouceList);
    }
    if (bNodeTopLevelDict.containsKey("comment"))
    {
        info.comment = BNodeRef<BNodeString>(bNodeTopLevelDict["comment"]).getString();
    }
    if (bNodeTopLevelDict.containsKey("creation date"))
    {
        info.creationDate = BNodeRef<BNodeInteger>(bNodeTopLevelDict["creation date"]).getInteger();
    }
    if (bNodeTopLevelDict.containsKey("created by"))
    {
        info.createdBy = BNodeRef<BNodeString>(bNodeTopLevelDict["created by"]).getString();
    }
    if (bNodeTopLevelDict.containsKey("info"))
    {
        BNodeDict &bNodeInfoDict = BNodeRef<BNodeDict>(bNodeTopLevelDict["info"]);
        if (bNodeInfoDict.containsKey("attr"))
        {
            info.attr = BNodeRef<BNodeString>(bNodeInfoDict["attr"]).getString();
        }
        if (bNodeInfoDict.containsKey("files"))
        {
            BNodeList &bNodeFilesList = BNodeRef<BNodeList>(bNodeInfoDict["files"]);
            for (auto &file : bNodeFilesList.getList())
            {
                BNodeDict &bNodeFileDict = BNodeRef<BNodeDict>(*file);
                TorrentFile fileEntry;
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
                info.files.push_back(fileEntry);
            }
        }
        if (bNodeInfoDict.containsKey("length"))
        {
            info.length = BNodeRef<BNodeInteger>(bNodeInfoDict["length"]).getInteger();
        }
        if (bNodeInfoDict.containsKey("name"))
        {
            info.name = BNodeRef<BNodeString>(bNodeInfoDict["name"]).getString();
        }
        if (bNodeInfoDict.containsKey("piece length"))
        {
            info.pieceLength = BNodeRef<BNodeInteger>(bNodeInfoDict["piece length"]).getInteger();
        }
        if (bNodeInfoDict.containsKey("pieces"))
        {
            info.pieces = BNodeRef<BNodeString>(bNodeInfoDict["pieces"]).getString();
        }
        if (bNodeInfoDict.containsKey("private"))
        {
            info.privateBitMask = BNodeRef<BNodeInteger>(bNodeInfoDict["private"]).getInteger();
        }
        if (bNodeInfoDict.containsKey("source"))
        {
            info.source = BNodeRef<BNodeString>(bNodeInfoDict["source"]).getString();
        }
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
void displayTorrentInfo(const TorrentInfo &info)
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
        TorrentInfo info;
        bEncode.decode(FileSource{"./testData/singlefile.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
        bEncode.decode(FileSource{"./testData/multifile.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
        bEncode.decode(FileSource{"./testData/file1.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
        bEncode.decode(FileSource{"./testData/file2.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
        bEncode.decode(FileSource{"./testData/file3.torrent"});
        info = getTorrentInfo(*bEncode);
        displayTorrentInfo(info);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }
    exit(EXIT_SUCCESS);
}