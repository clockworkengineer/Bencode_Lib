#pragma once
//
// C++ STL
//
#include <fstream>
#include <string>
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
        void add(const std::string &sourceBuffer) override
        {
            m_encodeBuffer = sourceBuffer;
        }
        std::string &getBuffer()
        {
            return (m_encodeBuffer);
        }

    private:
        std::string m_encodeBuffer;
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