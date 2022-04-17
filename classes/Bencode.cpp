//
// Class: Bencode
//
// Description: Class to perform Bencode encoding encode/decode to/from
// a byte buffer or file. It is also  possible to customize this with the
// ISource and IDestination interfaces if required. Although bencoded
// data is treated as std::byte externally this library uses char and
// std::string internally.Note: At present it will report incorrect Bencode
// syntax but will not be specific about what error has occurred; this
// is reasoned to add too much overhead to the process of parsing for the
// requirements of this library (this might change in future versions).
// For an indepth desciption of the Bencoding specification refer to its
// wiki page at https://en.wikipedia.org/wiki/Bencode.
//
// Dependencies:   C17++ - Language standard features used.
//
// =================
// CLASS DEFINITIONS
// =================
#include "BencodeConfig.hpp"
#include "Bencode.hpp"
// ====================
// CLASS IMPLEMENTATION
// ====================
//
// C++ STL
//
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
    /// <summary>
    /// Extract a positive Integer from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Positive integers value.</returns>
    long Bencode::extractPositiveInteger(ISource &source)
    {
        std::string buffer;
        while (source.more() && std::isdigit(static_cast<char>(source.current())))
        {
            buffer += static_cast<char>(source.current());
            source.next();
        }
        return (std::stol(buffer));
    }
    /// <summary>
    /// Extract a byte string from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>String value decoded.</returns>
    std::string Bencode::extractString(ISource &source)
    {
        long stringLength = extractPositiveInteger(source);
        if (source.current() != static_cast<std::byte>(':'))
        {
            throw SyntaxError();
        }
        source.next();
        std::string buffer;
        while (stringLength-- > 0)
        {
            buffer += static_cast<char>(source.current());
            source.next();
        }
        return (buffer);
    }
    /// <summary>
    /// Decode a byte string from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>String BNode.</returns>
    BNodePtr Bencode::decodeString(ISource &source)
    {
        return (std::make_unique<BNodeString>(extractString(source)));
    }
    /// <summary>
    /// Decode an integer from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Integer BNode.</returns>
    BNodePtr Bencode::decodeInteger(ISource &source)
    {
        long integer = 1;
        source.next();
        if (source.current() == static_cast<std::byte>('-'))
        {
            source.next();
            integer = -1;
        }
        integer *= extractPositiveInteger(source);
        if (source.current() != static_cast<std::byte>('e'))
        {
            throw SyntaxError();
        }
        source.next();
        return (std::make_unique<BNodeInteger>(integer));
    }
    /// <summary>
    /// Decode a dictionary from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Dictionary BNode.</returns>
    BNodePtr Bencode::decodeDictionary(ISource &source)
    {
        BNodePtr bNode = std::make_unique<BNodeDict>();
        source.next();
        while (source.more() && source.current() != static_cast<std::byte>('e'))
        {
            std::string key = extractString(source);
            BNodeRef<BNodeDict>(*bNode).addEntry(key, decodeBNodes(source));
        }
        if (source.current() != static_cast<std::byte>('e'))
        {
            throw SyntaxError();
        }
        source.next();
        return (bNode);
    }
    /// <summary>
    /// Decode a list from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>List BNode.</returns>
    BNodePtr Bencode::decodeList(ISource &source)
    {
        BNodePtr bNode = std::make_unique<BNodeList>();
        source.next();
        while (source.more() && source.current() != static_cast<std::byte>('e'))
        {
            BNodeRef<BNodeList>(*bNode).addEntry(decodeBNodes(source));
        }
        if (source.current() != static_cast<std::byte>('e'))
        {
            throw SyntaxError();
        }
        source.next();
        return (bNode);
    }
    /// <summary>
    /// Decode a BNode from the input stream of characters referenced by ISource.In order to traverse
    //  and decode complex encodings this method is called recursively to build up a BNode structure.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Root BNode.</returns>
    BNodePtr Bencode::decodeBNodes(ISource &source)
    {
        switch (static_cast<char>(source.current()))
        {
        // Dictionary BNode
        case 'd':
            return (decodeDictionary(source));
        // List BNode
        case 'l':
            return (decodeList(source));
        // Integer BNode
        case 'i':
            return (decodeInteger(source));
        // String BNode
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (decodeString(source));
        }
        throw SyntaxError();
    }
    /// <summary>
    /// Recursively traverse a BNode structure and produce an Bencode encoding of it on the output
    //  stream referenced through the IDestination interface.
    /// </summary>
    /// <param name="bNode">Pointer to root of current BNode structure.</param>
    /// <param name="desination ">Pointer to interface used to facilitate the output stream.</param>
    /// <returns></returns>
    void Bencode::encodeBNodes(BNode *bNode, IDestination &destination)
    {
        switch (bNode->nodeType)
        {
        case BNodeType::dictionary:
            destination.add("d");
            for (auto &bNodeEntry : BNodeRef<BNodeDict>(*bNode).getDict())
            {
                destination.add(std::to_string(bNodeEntry.first.length()) + ":" + bNodeEntry.first);
                encodeBNodes(bNodeEntry.second.get(), destination);
            }
            destination.add("e");
            break;
        case BNodeType::list:
            destination.add("l");
            for (auto &bNodeEntry : BNodeRef<BNodeList>(*bNode).getArray())
            {
                encodeBNodes(bNodeEntry.get(), destination);
            }
            destination.add("e");
            break;
        case BNodeType::integer:
            destination.add("i" + std::to_string(BNodeRef<BNodeInteger>(*bNode).getInteger()) + "e");
            break;
        case BNodeType::string:
        {
            std::string stringToEncode = BNodeRef<BNodeString>(*bNode).getString();
            destination.add(std::to_string(static_cast<int>(stringToEncode.length())) + ":" + stringToEncode);
            break;
        }
        default:
            throw std::runtime_error("Unknown BNode type encountered during encode.");
        }
    }
    // ==============
    // PUBLIC METHODS
    // ==============
    /// <summary>
    /// Decode Bencoded byte string pointed to by source stream into BNode(s).
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    void Bencode::decode(ISource &source)
    {
        m_bNodeRoot = decodeBNodes(source);
    }
    void Bencode::decode(ISource &&source)
    {
        m_bNodeRoot = decodeBNodes(source);
    }
    /// <summary>
    /// Take BNode structure and create an Bencode encoding for it in the destination stream.
    /// </summary>
    /// <param name="bNodeRoot">Bnode structure root.</param>
    /// <param name="desination ">Pointer to interface used to facilitate the output stream.</param>
    void Bencode::encode(IDestination &destination)
    {
        encodeBNodes(m_bNodeRoot.get(), destination);
    }
    /// <summary>
    /// Return root BNode structure of decoded string.
    /// </summary>
    /// <returns>BNode structure root.</returns>
    BNode *Bencode::getRoot()
    {
        return (m_bNodeRoot.get());
    }
} // namespace BencodeLib
