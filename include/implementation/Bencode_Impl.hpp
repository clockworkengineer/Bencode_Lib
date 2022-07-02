#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
#include <limits>
#include <array>
#include <sstream>
// =============================
// Source/Destination interfaces
// =============================
#include "ISource.hpp"
#include "IDestination.hpp"
// =======
// Bencode
// =======
#include "Bencode_Config.hpp"
#include "Bencode_Types.hpp"
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
        // ======================
        // CONSTRUCTOR/DESTRUCTOR
        // ======================
        Bencode_Impl() = default;
        Bencode_Impl(const Bencode_Impl &other) = delete;
        Bencode_Impl &operator=(const Bencode_Impl &other) = delete;
        Bencode_Impl(Bencode_Impl &&other) = delete;
        Bencode_Impl &operator=(Bencode_Impl &&other) = delete;
        ~Bencode_Impl() = default;
        // ==============
        // PUBLIC METHODS
        // ==============
        BNode::Ptr decode(ISource &source);
        void encode(const BNode &bNodeRoot, IDestination &destination);
        std::string version();
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
        static int64_t extractInteger(ISource &source);
        static std::string extractString(ISource &source);
        static BNode::Ptr decodeString(ISource &source);
        static BNode::Ptr decodeInteger(ISource &source);
        static BNode::Ptr decodeDictionary(ISource &source);
        static BNode::Ptr decodeList(ISource &source);
        static BNode::Ptr decodeBNodes(ISource &source);
        static void encodeBNodes(const BNode &bNode, IDestination &destination);
        // =================
        // PRIVATE VARIABLES
        // =================
    };
} // namespace BencodeLib
