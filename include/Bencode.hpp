#pragma once
//
// C++ STL
//
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
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

    class Bencode
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        //
        // Bencode syntax error.
        //
        struct SyntaxError : public std::exception
        {
        public:
            explicit SyntaxError(std::string errorMessage = "Bencode Error : Syntax error detected.") : errorMessage(std::move(errorMessage)) {}
            [[nodiscard]] const char *what() const noexcept override
            {
                return (errorMessage.c_str());
            }

        private:
            const std::string errorMessage;
        };
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
        void decode(ISource &&source);
        void encode(IDestination &destination);
        void encode(IDestination &&destination);
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
        void encodeBNodes(BNode *bNode, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
        BNodePtr m_bNodeRoot;
    };
} // namespace BencodeLib
