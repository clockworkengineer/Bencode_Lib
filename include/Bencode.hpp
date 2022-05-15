#pragma once
//
// C++ STL
//
#include <string>
#include <stdexcept>
#include <memory>
//
// Bencode version
//
#include "Bencode_Config.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ====================
    // Bencode syntax error
    // ====================
    struct SyntaxError : public std::exception
    {
    public:
        explicit SyntaxError(std::string errorMessage = "Syntax error detected.") : errorMessage(std::move(errorMessage)) {}
        [[nodiscard]] const char *what() const noexcept override
        {
            return ((std::string("Bencode Error:")+errorMessage).c_str());
        }
    private:
        const std::string errorMessage;
    };
    // ====================
    // Forward declarations
    // ====================
    class ISource;
    class IDestination;
    class Bencode_Impl;
    struct BNode;
    // ================
    // CLASS DEFINITION
    // ================
    class Bencode
    {
    public:
        // ==========================
        // PUBLIC TYPES AND CONSTANTS
        // ==========================
        // ============
        // CONSTRUCTORS
        // ============
        Bencode();
        // ==========
        // DESTRUCTOR
        // ==========
        ~Bencode();
        // ==============
        // PUBLIC METHODS
        // ==============
        void decode(ISource &source);
        void decode(ISource &&source);
        void encode(IDestination &destination);
        void encode(IDestination &&destination);
        BNode &root();
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
        // =================
        // PRIVATE VARIABLES
        // =================
        std::unique_ptr<Bencode_Impl> m_implementation;
    };
} // namespace BencodeLib
