#pragma once
//
// C++ STL
//
#include <string>
#include <map>
#include <vector>
#include <memory>
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
        BNode(BNodeType nodeType = BNodeType::base) : nodeType(nodeType)
        {
        }
        virtual ~BNode() {}
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
        BNodeDict() : BNode(BNodeType::dictionary) {}
        bool containsKey(const std::string &key) const
        {
            return (m_value.count(key) > 0);
        }
        int size() const
        {
            return ((int)m_value.size());
        }
        void addEntry(const std::string key, BNodePtr entry)
        {
            m_value[key] = std::move(entry);
        }
        BNode *getEntry(const std::string &key)
        {
            return (m_value[key].get());
        }
        std::map<std::string, BNodePtr> &getDict() 
        {
            return (m_value);
        }
    protected:
        std::map<std::string, BNodePtr> m_value;
    };
    //
    // List BNode.
    //
    struct BNodeList : BNode
    {
        BNodeList() : BNode(BNodeType::list) {}
        int size() const
        {
            return (static_cast<int>(m_value.size()));
        }
        void addEntry(BNodePtr jNode)
        {
            m_value.push_back(std::move(jNode));
        }
        std::vector<BNodePtr> &getArray()
        {
            return (m_value);
        }
        BNode *getEntry(int index)
        {
            return (m_value[index].get());
        }
    protected:
        std::vector<BNodePtr> m_value;
    };
    //
    // Integer BNode.
    //
    struct BNodeInteger : BNode
    {
        BNodeInteger() : BNode(BNodeType::integer) {}
        BNodeInteger(long value) : BNode(BNodeType::integer)
        {
            m_value = value;
        }
        long getInteger() const
        {
            return (m_value);
        }
        void setInteger(long value)
        {
            m_value = value;
        }
    protected:
        long m_value = 0;
    };
    //
    // String BNode.
    //
    struct BNodeString : BNode
    {
    public:
        BNodeString() : BNode(BNodeType::string) {}
        BNodeString(const std::string &value) : BNode(BNodeType::string)
        {
            this->m_value = value;
        }
        std::string getString() const
        {
            return (m_value);
        }
        void setString(const std::string &value)
        {
            m_value = value;
        }
    protected:
        std::string m_value;
    };
    //
    // Convert base BNode reference
    //
    template <typename T>
    T &BNodeRef(BNode &bNode)
    {
        return (static_cast<T &>(bNode));
    }
    //
    // Index overloads
    //
    inline BNode &BNode::operator[](const std::string &key) //Dicionary
    {
        if (nodeType == BNodeType::dictionary)
        {
            if (BNodeRef<BNodeDict>(*this).containsKey(key))
            {
                return (*((BNodeRef<BNodeDict>(*this).getEntry(key))));
            }
        }
        throw std::runtime_error("Invalid key used in dictionary.");
    }
    inline BNode &BNode::operator[](int index) // List
    {
        if (nodeType == BNodeType::list)
        {
            if ((index >= 0) && (index < (static_cast<int>(BNodeRef<BNodeList>(*this).size()))))
            {
                return (*((BNodeRef<BNodeList>(*this).getEntry(index))));
            }
        }
        throw std::runtime_error("Invalid index used in list.");
    }
} // namespace BencodeLib