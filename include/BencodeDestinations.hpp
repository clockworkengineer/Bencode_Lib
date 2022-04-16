#pragma once
//
// C++ STL
//
#include <fstream>
#include <string>
// 
//  Bencoded data structure
//
#include "Bencoding.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    //
    // Destination classes for encoders.
    //
    class BufferDestination : public IDestination
    {
    public:
        BufferDestination()
        {
        }
        void add(const std::string &bytes) override
        {
            for (auto byte : bytes)
            {
                m_encodeBuffer.add(static_cast<std::byte>(byte));
            }
        }
        Bencoding getBuffer() const
        {
            return (m_encodeBuffer);
        }
    private:
        Bencoding m_encodeBuffer;
    };
    class FileDestination : public IDestination
    {
    public:
        FileDestination(const std::string &desinationFileName)
        {
            m_destination.open(desinationFileName.c_str(), std::ios_base::binary);
            if (!m_destination.is_open())
            {
                throw std::runtime_error("Bencode file output stream failed to open or could not be created.");
            }
        }
        void add(const std::string &bytes) override
        {
            m_destination.write(bytes.c_str(), bytes.length());
            m_destination.flush();
        }
    private:
        std::ofstream m_destination;
    };
} // namespace BencodeLib