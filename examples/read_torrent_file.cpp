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
    std::string attr;
    std::string comment;
    std::uint64_t createDate;
    std::string createdBy;
    std::vector<TorrentFile> files;
    std::uint64_t length;
    std::string name;
    std::uint64_t pieceLength;
    std::string pieces;
    std::uint64_t privateBitMask;
    std::string source;
    std::string urlList;
};

// ===============
// LOCAL FUNCTIONS
// ===============

TorrentInfo getTorrentInfo(BNode &bNode)
{
    try
    {
        TorrentInfo info;
        if (bNode.nodeType != BNodeType::dictionary)
        {
            throw std::exception("Valid torrent file not found.");
        }
        BNodeDict &bNodeDict = BNodeRef<BNodeDict>(bNode);
        if (bNodeDict.containsKey("announce"))
        {
            info.announce = BNodeRef<BNodeString>(bNodeDict["announce"]).getString();
        }
        if (bNodeDict.containsKey("comment"))
        {
            info.comment = BNodeRef<BNodeString>(bNodeDict["comment"]).getString();
        }
        if (bNodeDict.containsKey("creation date"))
        {
            info.createDate = BNodeRef<BNodeInteger>(bNodeDict["creation date"]).getInteger();
        }
        if (bNodeDict.containsKey("created by"))
        {
            info.createdBy = BNodeRef<BNodeString>(bNodeDict["created by"]).getString();
        }
        if (bNodeDict.containsKey("info"))
        {
            BNodeDict &bNodeInfoDict = BNodeRef<BNodeDict>(bNodeDict["info"]);
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
                    // for (auto &[key, bNodePtr] : bNodeFileDict.getDict())
                    // {
                    //     std::cout << key << " ";
                    //     if (key == "length")
                    //     {
                    //         uint64_t length = BNodeRef<BNodeInteger>(*bNodePtr).getInteger();
                    //         std::cout << length << "\n";
                    //     }
                    //     else if (key == "path")
                    //     {
                    //         BNodeList &bNodePathList = BNodeRef<BNodeList>(*bNodePtr);
                    //         std::string path{};
                    //         for (auto &folder : bNodePathList.getList())
                    //         {
                    //             path += "/"+ BNodeRef<BNodeString>(*folder).getString();
                    //         }
                    //         std::cout << path << "\n";
                    //     }
                    // }
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
                BNodeString &bNodeString = BNodeRef<BNodeString>(bNodeInfoDict["pieces"]);
                info.pieces = bNodeString.getString();
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
        if (bNodeDict.containsKey("url-list"))
        {
            info.urlList = BNodeRef<BNodeString>(bNodeDict["url-list"]).getString();
        }

        return (info);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    Bencode bEncode;
     bEncode.decode(FileSource{"./testData/singlefile.torrent"});
    //bEncode.decode(FileSource{"./testData/multifile.torrent"});
    TorrentInfo info = getTorrentInfo(*bEncode);
    std::cout << "announce [" << info.announce << "]\n";
    std::cout << "attr [" << info.attr << "]\n";
    std::cout << "comment [" << info.comment << "]\n";
    std::cout << "creation_date [" << info.createDate << "]\n";
    std::cout << "created_by [" << info.createdBy << "]\n";
    std::cout << "length [" << info.length << "]\n";
    std::cout << "name [" << info.name << "]\n";
    std::cout << "piece length [" << info.pieceLength << "]\n";
    // std::cout << "pieces [" << info.pieces << "]\n";
    std::cout << "private [" << info.privateBitMask << "]\n";
    std::cout << "source [" << info.source << "]\n";
    std::cout << "url_list [" << info.urlList << "]\n";
    for (auto file : info.files)
    {
        std::cout << "path [ " << file.path << "] length [" << file.length << "]\n";
    }
    exit(EXIT_SUCCESS);
}