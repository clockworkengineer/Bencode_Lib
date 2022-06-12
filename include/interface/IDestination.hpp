#pragma once
//
// C++ STL
//
#include <string>
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    //
    // Interface for writing destination stream during Bencode encoding.
    //
    class IDestination
    {
    public:
        virtual ~IDestination() = default;
        virtual void add(const std::string &bytes) = 0;
        virtual void add (char ch)= 0;
    };
} // namespace BencodeLib