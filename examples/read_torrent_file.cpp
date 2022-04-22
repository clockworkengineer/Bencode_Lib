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

void getInfo(BNodeDict &bNode)
{
    for (const auto &[key, bNodePtr] : bNode.getDict())
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
            throw std::exception("Torrent file not found.");
        }

        BNodeDict &bNodeDict = BNodeRef<BNodeDict>(bNode);

        if (bNodeDict.containsKey("announce"))
        {
            std::cout << BNodeRef<BNodeString>(bNodeDict["announce"]).getString() << "\n";
        }
        else
        {
            throw std::exception("Missing annouce field.");
        }

        if (bNodeDict.containsKey("comment"))
        {
            std::cout << BNodeRef<BNodeString>(bNodeDict["comment"]).getString() << "\n";
        }
        else
        {
            throw std::exception("Missing comment field.");
        }

        if (bNodeDict.containsKey("created by"))
        {
            std::cout << BNodeRef<BNodeString>(bNodeDict["created by"]).getString() << "\n";
        }
        else
        {
            throw std::exception("Missing created by field.");
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
            std::cout << BNodeRef<BNodeString>(bNodeDict["url-list"]).getString() << "\n";
        }
        else
        {
            throw std::exception("Missing url-list field.");
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

    bEncode.decode(FileSource{"./testData/singlefile.torrent"});

    getTorrentInfo(*bEncode);

    exit(EXIT_SUCCESS);
}