//
// Class: Bencode
//
// Description: Class to perform Bencode encoding encode/decode to/from
// a byte buffer or file. It is also  possible to customize this with the
// ISource and IDestination interfaces if required. Although Bencoded
// data is treated as std::byte externally this library uses char and
// std::string internally.Note: At present it will report incorrect Bencode
// syntax but will not be specific about what error has occurred; this
// is reasoned to add too much overhead to the process of parsing for the
// requirements of this library (this might change in future versions).
// For an in-depth description of the Bencode specification refer to its
// Wiki page at https://en.wikipedia.org/wiki/Bencode.
//
// Dependencies:   C20++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "BencodeConfig.hpp"
#include "Bencode.hpp"
#include "BencodeImplementation.hpp"
//
// Source/Destination interfaces
//
// #include "ISource.hpp"
// #include "IDestination.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
//
// C++ STL
//
#include <limits>
#include <array>
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ===========================
    // PRIVATE TYPES AND CONSTANTS
    // ===========================
    // ==========================
    // PUBLIC TYPES AND CONSTANTS
    // ==========================
    // ========================
    // PRIVATE STATIC VARIABLES
    // ========================
    // =======================
    // PUBLIC STATIC VARIABLES
    // =======================
    // ===============
    // PRIVATE METHODS
    // ===============
    // ==============
    // PUBLIC METHODS
    // ==============
    Bencode::Bencode()
    {
        m_imp = std::make_unique<BencodeImplementation>();
    }

    Bencode::~Bencode()
    {
    };
    /// <summary>
    /// Decode Bencoded byte string pointed to by source stream into BNode(s).
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns></returns>
    void Bencode::decode(ISource &source)
    {
        m_imp->decode(source);
    }
    void Bencode::decode(ISource &&source)
    {
        m_imp->decode(source);
    }
    /// <summary>
    /// Take BNode structure and create an Bencode encoding for it in the destination stream.
    /// </summary>
    /// <param name="bNodeRoot">BNode structure root.</param>
    /// <param name="destination ">Pointer to interface used to facilitate the output stream.</param>
    /// <returns></returns>
    void Bencode::encode(IDestination &destination)
    {
        m_imp->encode(destination);
    }
    void Bencode::encode(IDestination &&destination)
    {
        m_imp->encode(destination);
    }
    BNode &Bencode::root()
    {
        return (m_imp->root());
    }
} // namespace BencodeLib
