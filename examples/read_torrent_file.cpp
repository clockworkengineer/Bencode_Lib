//
// Program: read_torrent_file
//
// Description: Use BencodeLib to read in torrent file details.
//
// ArchiveFolder Example Application
//
// Dependencies: C20++.
//

// =============
// INCLUDE FILES
// =============

//
// C++ STL
//

#include <cstdlib>
#include <iostream>
#include <thread>
#include <iomanip>
#include <fstream>

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

// ============================
// ===== MAIN ENTRY POINT =====
// ============================

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    FileSource torrentSource { "./testData/singlefile.torrent"};
    exit(EXIT_SUCCESS);
}