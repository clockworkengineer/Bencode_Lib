#pragma once
//
// C++ STL
//
#include <string>
#include <stdexcept>
#include <limits>
#include <array>
//
// Source/Destination interfaces
//
#include "ISource.hpp"
#include "IDestination.hpp"
//
// BNode types
//
#include "BNode.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class BencodeImplementation
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ============
        // CONSTRUCTORS
        // ============
        // ==========
        // DESTRUCTOR
        // ==========
        // ==============
        // PUBLIC METHODS
        // ==============
        void decode(ISource &source);
        void encode(IDestination &destination);
        BNode &root() { return (*m_bNodeRoot); }
        // ================
        // PUBLIC VARIABLES
        // ================
    private:
        // ===========================
        // PRIVATE TYPES AND CONSTANTS
        // ===========================
        // ===========================================
        // DISABLED CONSTRUCTORS/DESTRUCTORS/OPERATORS
        // ===========================================
        // ===============
        // PRIVATE METHODS
        // ===============
        int64_t extractInteger(ISource &source);
        std::string extractString(ISource &source);
        BNodePtr decodeString(ISource &source);
        BNodePtr decodeInteger(ISource &source);
        BNodePtr decodeDictionary(ISource &source);
        BNodePtr decodeList(ISource &source);
        BNodePtr decodeBNodes(ISource &source);
        void encodeBNodes(BNode &bNode, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        BNodePtr m_bNodeRoot;
    };
} // namespace BencodeLib
