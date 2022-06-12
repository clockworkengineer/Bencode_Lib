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
#include "Bencode_Nodes.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ================
    // CLASS DEFINITION
    // ================
    class Bencode_Impl
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
        BNode::Ptr decodeString(ISource &source);
        BNode::Ptr decodeInteger(ISource &source);
        BNode::Ptr decodeDictionary(ISource &source);
        BNode::Ptr decodeList(ISource &source);
        BNode::Ptr decodeBNodes(ISource &source);
        void encodeBNodes(const BNode &bNode, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        BNode::Ptr m_bNodeRoot;
    };
} // namespace BencodeLib
