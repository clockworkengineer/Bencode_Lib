#pragma once
//
// C++ STL
//
#include <fstream>
#include <string>
#include <vector>

//
// IDestination interface
//
#include "IDestination.hpp"

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
        BufferDestination() = default;
        void add(const std::string &sourceBuffer) override
        {
            for (auto ch : sourceBuffer)
            {
                m_encodeBuffer.push_back(static_cast<std::byte>(ch));
            }
        }
        void add(const char ch) override
        {
            m_encodeBuffer.push_back(static_cast<std::byte>(ch));
        }
        std::vector<std::byte> &getBuffer()
        {
            return (m_encodeBuffer);
        }

    private:
        std::vector<std::byte> m_encodeBuffer;
    };
    class FileDestination : public IDestination
    {
    public:
        explicit FileDestination(const std::string &destinationFileName)
        {
            m_destination.open(destinationFileName.c_str(), std::ios_base::binary);
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

        void add(const char ch) override
        {
            m_destination.put(ch);
            m_destination.flush();
        }

    private:
        std::ofstream m_destination;
    };
} // namespace BencodeLib