#pragma once
//
// C++ STL
//
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    //
    // BNode types.
    //
    enum class BNodeType
    {
        base = 0,
        dictionary = 1,
        list = 2,
        integer = 3,
        string = 4
    };
    //
    // Base BNode
    //
    struct BNode
    {
        struct Error : public std::exception
        {
        public:
            explicit Error(std::string errorMessage) : errorMessage(std::move(errorMessage)) {}
            [[nodiscard]] const char *what() const noexcept override
            {
                return (errorMessage.c_str());
            }

        private:
            const std::string errorMessage;
        };
        explicit BNode(BNodeType nodeType = BNodeType::base) : nodeType(nodeType)
        {
        }
        virtual ~BNode() = default;
        BNode &operator[](const std::string &key);
        BNode &operator[](int index);
        const BNodeType nodeType;
    };
    //
    // Pointer to BNode
    //
    using BNodePtr = std::unique_ptr<BNode>;
    //
    // Dictionary BNode.
    //
    struct BNodeDict : BNode
    {
        using Entry = std::pair<std::string, BNodePtr>;
        explicit BNodeDict(std::vector<BNodeDict::Entry> &value) : BNode(BNodeType::dictionary),
                                                                   m_value(std::move(value))
        {
        }
        [[nodiscard]] bool contains(const std::string &key) const
        {
            return (get(key) != nullptr);
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        [[nodiscard]] BNode *get(const std::string &key) const
        {
            if (auto it = std::find_if(m_value.begin(), m_value.end(), [&key](const Entry &entry) -> bool
                                       { return (entry.first == key); });
                it != m_value.end())
            {
                return (it->second.get());
            }
            return (nullptr);
        }
        [[nodiscard]] const std::vector<Entry> &dictionary() const
        {
            return (m_value);
        }

    private:
        const std::vector<BNodeDict::Entry> m_value;
    };
    //
    // List BNode.
    //
    struct BNodeList : BNode
    {
        explicit BNodeList(std::vector<BNodePtr> &value) : BNode(BNodeType::list),
                                                           m_value(std::move(value))
        {
        }
        [[nodiscard]] int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        [[nodiscard]] const std::vector<BNodePtr> &list() const
        {
            return (m_value);
        }
        [[nodiscard]] BNode *get(int index) const
        {
            return (m_value[index].get());
        }

    private:
        const std::vector<BNodePtr> m_value;
    };
    //
    // Integer BNode.
    //
    struct BNodeInteger : BNode
    {
        explicit BNodeInteger(int64_t value) : BNode(BNodeType::integer),
                                               m_value(value)
        {
        }
        [[nodiscard]] int64_t getInteger() const
        {
            return (m_value);
        }

    private:
        const int64_t m_value = 0;
    };
    //
    // String BNode.
    //
    struct BNodeString : BNode
    {
        explicit BNodeString(std::string value) : BNode(BNodeType::string),
                                                  m_value(std::move(value))
        {
        }
        [[nodiscard]] std::string getString() const
        {
            return (m_value);
        }

    private:
        const std::string m_value;
    };
    //
    // Convert base BNode reference
    //
    template <typename T>
    T &BNodeRef(BNode &bNode)
    {
        if constexpr (std::is_same_v<T, BNodeString>)
        {
            if (bNode.nodeType != BNodeType::string)
            {
                throw BNode::Error("BNode Error: Node not a string.");
            }
        }
        else if constexpr (std::is_same_v<T, BNodeInteger>)
        {
            if (bNode.nodeType != BNodeType::integer)
            {
                throw BNode::Error("BNode Error: Node not an integer.");
            }
        }
        else if constexpr (std::is_same_v<T, BNodeList>)
        {
            if (bNode.nodeType != BNodeType::list)
            {
                throw BNode::Error("BNode Error: Node not a list.");
            }
        }
        else if constexpr (std::is_same_v<T, BNodeDict>)
        {
            if (bNode.nodeType != BNodeType::dictionary)
            {
                throw BNode::Error("BNode Error: Node not a dictionary.");
            }
        }
        return (static_cast<T &>(bNode));
    }
    //
    // Index overloads
    //
    inline BNode &BNode::operator[](const std::string &key) // Dictionary
    {
        if (nodeType == BNodeType::dictionary)
        {
            if (BNodeRef<BNodeDict>(*this).contains(key))
            {
                return (*((BNodeRef<BNodeDict>(*this).get(key))));
            }
            throw BNode::Error("BNode Error: Invalid key used in dictionary.");
        }
        throw BNode::Error("BNode Error: Node not a dictionary.");
    }
    inline BNode &BNode::operator[](int index) // List
    {
        if (nodeType == BNodeType::list)
        {
            if ((index >= 0) && (index < (static_cast<int>(BNodeRef<BNodeList>(*this).size()))))
            {
                return (*((BNodeRef<BNodeList>(*this).get(index))));
            }
            throw BNode::Error("BNode Error: Invalid index used in list.");
        }
        throw BNode::Error("BNode Error: Node not a list.");
    }
} // namespace BencodeLib