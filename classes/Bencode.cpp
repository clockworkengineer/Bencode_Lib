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
    /// ********************************************************************************
    /// <summary>
    /// Extract a Integer from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Positive integers value.</returns>
    /// ********************************************************************************
    int64_t Bencode::extractInteger(ISource &source)
    {
        // Number size of 64 bit int +2 for sign and terminating null
        std::array<char, std::numeric_limits<int64_t>::digits10 + 2> number{};
        int digits = 0;
        if (source.current() == '-')
        {
            number[digits++] = source.current();
            source.next();
        }
        while (source.more() && (std::isdigit(source.current()) != 0))
        {
            // Number too large to be  in buffer
            if (digits == number.size())
            {
                throw SyntaxError();
            }
            number[digits++] = source.current();
            source.next();
        }
        // Check integer has no leading zero and is not empty ('ie')
        if ((number[0] == '0' && digits > 1) || (digits == 0))
        {
            throw SyntaxError();
        }
        // Check-for -0
        if ((number[0] == '-') && (number[1] == '0') && (digits == 2))
        {
            throw SyntaxError();
        }
        return (std::stoll(&number[0]));
    }
    /// ********************************************************************************
    /// <summary>
    /// Extract a byte string from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>String value decoded.</returns>
    /// ********************************************************************************
    std::string Bencode::extractString(ISource &source)
    {
        int64_t stringLength = extractInteger(source);
        if (source.current() != ':')
        {
            throw SyntaxError();
        }
        source.next();
        std::string buffer;
        while (stringLength-- > 0)
        {
            buffer += source.current();
            source.next();
        }
        return (buffer);
    }
    /// ********************************************************************************
    /// <summary>
    /// Decode a byte string from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>String BNode.</returns>
    /// ********************************************************************************
    BNodePtr Bencode::decodeString(ISource &source)
    {
        return (std::make_unique<BNodeString>(extractString(source)));
    }
    /// ********************************************************************************
    /// <summary>
    /// Decode an integer from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Integer BNode.</returns>
    /// ********************************************************************************
    BNodePtr Bencode::decodeInteger(ISource &source)
    {
        source.next();
        int64_t integer = extractInteger(source);
        if (source.current() != 'e')
        {
            throw SyntaxError();
        }
        source.next();
        return (std::make_unique<BNodeInteger>(integer));
    }
    /// ********************************************************************************
    /// <summary>
    /// Decode a dictionary from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Dictionary BNode.</returns>
    /// ********************************************************************************
    BNodePtr Bencode::decodeDictionary(ISource &source)
    {
        std::vector<BNodeDict::Entry> dictionary;
        std::string lastKey{};
        source.next();
        while (source.more() && source.current() != 'e')
        {
            std::string key = extractString(source);
            // Check keys in lexical order
            if (lastKey > key)
            {
                throw SyntaxError();
            }
            lastKey = key;
            // Check key not duplicate and insert
            if (std::find_if(dictionary.begin(), dictionary.end(), [&key](const BNodeDict::Entry &entry) -> bool
                                 { return (entry.first == key); }) == dictionary.end())
            {
                dictionary.emplace_back(key, decodeBNodes(source));
            }
            else
            {
                throw SyntaxError();
            }
        }
        if (source.current() != 'e')
        {
            throw SyntaxError();
        }
        source.next();
        return (std::make_unique<BNodeDict>(dictionary));
    }
    /// ********************************************************************************
    /// <summary>
    /// Decode a list from the input stream of characters referenced by ISource.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>List BNode.</returns>
    /// ********************************************************************************
    BNodePtr Bencode::decodeList(ISource &source)
    {
        std::vector<BNodePtr> list;
        source.next();
        while (source.more() && source.current() != 'e')
        {
            list.emplace_back(decodeBNodes(source));
        }
        if (source.current() != 'e')
        {
            throw SyntaxError();
        }
        source.next();
        return (std::make_unique<BNodeList>(list));
    }
    /// ********************************************************************************
    /// <summary>
    /// Decode a BNode from the input stream of characters referenced by ISource.In order to traverse
    //  and decode complex encodings this method is called recursively to build up a BNode structure.
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns>Root BNode.</returns>
    /// ********************************************************************************
    BNodePtr Bencode::decodeBNodes(ISource &source)
    {
        switch (source.current())
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
    /// ********************************************************************************
    /// <summary>
    /// Recursively traverse a BNode structure and produce an Bencode encoding of it on
    /// the output stream referenced through the IDestination interface.
    /// </summary>
    /// <param name="bNode">Pointer to root of current BNode structure.</param>
    /// <param name="desination ">Pointer to interface used to facilitate the output stream.</param>
    /// <returns></returns>
    /// ********************************************************************************
    void Bencode::encodeBNodes(BNode *bNode, IDestination &destination)
    {
        if (bNode == nullptr)
        {
            throw std::runtime_error("No Bencoded data to encode.");
        }
        switch (bNode->nodeType)
        {
        case BNodeType::dictionary:
            destination.add("d");
            for (const auto &bNodeEntry : BNodeRef<BNodeDict>(*bNode).getDict())
            {
                destination.add(std::to_string(bNodeEntry.first.length()) + ":" + bNodeEntry.first);
                encodeBNodes(bNodeEntry.second.get(), destination);
            }
            destination.add("e");
            break;
        case BNodeType::list:
            destination.add("l");
            for (const auto &bNodeEntry : BNodeRef<BNodeList>(*bNode).getList())
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
    /// ********************************************************************************
    /// <summary>
    /// Decode Bencoded byte string pointed to by source stream into BNode(s).
    /// </summary>
    /// <param name="source">Pointer to input interface used to decode Bencoded stream.</param>
    /// <returns></returns>
    /// ********************************************************************************
    void Bencode::decode(ISource &source)
    {
        m_bNodeRoot = decodeBNodes(source);
    }
    void Bencode::decode(ISource &&source)
    {
        m_bNodeRoot = decodeBNodes(source);
    }
    /// ********************************************************************************
    /// <summary>
    /// Take BNode structure and create an Bencode encoding for it in the destination stream.
    /// </summary>
    /// <param name="bNodeRoot">BNode structure root.</param>
    /// <param name="desination ">Pointer to interface used to facilitate the output stream.</param>
    /// <returns></returns>
    /// ********************************************************************************
    void Bencode::encode(IDestination &destination)
    {
        encodeBNodes(m_bNodeRoot.get(), destination);
    }
    void Bencode::encode(IDestination &&destination)
    {
        encodeBNodes(m_bNodeRoot.get(), destination);
    }
} // namespace BencodeLib
