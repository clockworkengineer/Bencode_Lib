#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
#include <memory>
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ====================
    // Forward declarations
    // ====================
    class Bencode_Impl;
    class ISource;
    class IDestination;
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
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        Bencode();
        Bencode(const Bencode &other) = delete;
        Bencode &operator=(const Bencode &other) = delete;
        Bencode(Bencode &&other) = delete;
        Bencode &operator=(Bencode &&other) = delete;
        ~Bencode();
        // ==============
        // PUBLIC METHODS
        // ==============
        void decode(ISource &source);
        void decode(ISource &&source);
        void encode(IDestination &destination);
        void encode(IDestination &&destination);
        std::string version() const;
        BNode &root();
        const BNode &root() const;
        // ================
        // PUBLIC VARIABLES
        // ================
    private:
        // ===========================
        // PRIVATE TYPES AND CONSTANTS
        // ===========================
        // ===============
        // PRIVATE METHODS
        // ===============
        // =================
        // PRIVATE VARIABLES
        // =================
        const std::unique_ptr<Bencode_Impl> m_implementation;
    };
} // namespace BencodeLib
