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
// ===============
// LOCAL FUNCTIONS
// ===============
void getInfo(BNodeDict &bNodeDict)
{
    if (bNodeDict.containsKey("attr"))
    {
        std::cout << "attr [" << BNodeRef<BNodeString>(bNodeDict["attr"]).getString() << "]\n";
    }
    if (bNodeDict.containsKey("length"))
    {
        std::cout << "length [" << BNodeRef<BNodeInteger>(bNodeDict["length"]).getInteger() << "]\n";
    }
    if (bNodeDict.containsKey("name"))
    {
        std::cout << "name [" << BNodeRef<BNodeString>(bNodeDict["name"]).getString() << "]\n";
    }
    if (bNodeDict.containsKey("piece length"))
    {
        std::cout << "piece length [" << BNodeRef<BNodeInteger>(bNodeDict["piece length"]).getInteger() << "]\n";
    }
    if (bNodeDict.containsKey("pieces"))
    {
        BNodeString &bNodeString = BNodeRef<BNodeString>(bNodeDict["pieces"]);
        if (bNodeString.nodeType == BNodeType::string)
        {
            std::cout << "PIECES\n";
        }
    }
    if (bNodeDict.containsKey("private"))
    {
        std::cout << "private [" << BNodeRef<BNodeInteger>(bNodeDict["private"]).getInteger() << "]\n";
    }
    if (bNodeDict.containsKey("source"))
    {
        std::cout << "source [" << BNodeRef<BNodeString>(bNodeDict["source"]).getString() << "]\n";
    }
    for (const auto &[key, bNodePtr] : bNodeDict.getDict())
    {
        std::cout << key << "\n";
    }
}
void getTorrentInfo(BNode &bNode)
{
    try
    {
        if (bNode.nodeType != BNodeType::dictionary)
        {
            throw std::exception("Valid torrent file not found.");
        }
        BNodeDict &bNodeDict = BNodeRef<BNodeDict>(bNode);
        if (bNodeDict.containsKey("announce"))
        {
            std::cout << "announce [" << BNodeRef<BNodeString>(bNodeDict["announce"]).getString() << "]\n";
        }
        if (bNodeDict.containsKey("comment"))
        {
            std::cout << "comment [" << BNodeRef<BNodeString>(bNodeDict["comment"]).getString() << "]\n";
        }
        if (bNodeDict.containsKey("creation date"))
        {
            std::cout << "creation date [" << BNodeRef<BNodeInteger>(bNodeDict["creation date"]).getInteger() << "]\n";
        }
        if (bNodeDict.containsKey("created by"))
        {
            std::cout << "created by [" << BNodeRef<BNodeString>(bNodeDict["created by"]).getString() << "]\n";
        }
        if (bNodeDict.containsKey("info"))
        {
            getInfo(BNodeRef<BNodeDict>(bNodeDict["info"]));
        }
        else
        {
            throw std::exception("Missing info field.");
        }
        if (bNodeDict.containsKey("url-list"))
        {
            std::cout << "url-list [" << BNodeRef<BNodeString>(bNodeDict["url-list"]).getString() << "]\n";
        }
        for (const auto &[key, bNodePtr] : bNodeDict.getDict())
        {
            std::cout << key << "\n";
        }
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
    //  bEncode.decode(FileSource{"./testData/singlefile.torrent"});
    bEncode.decode(FileSource{"./testData/multifile.torrent"});
    getTorrentInfo(*bEncode);
    exit(EXIT_SUCCESS);
}