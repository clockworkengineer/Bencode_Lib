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
    // Source classes for decoders.
    //
    class BufferSource : public ISource
    {
    public:
        BufferSource(const Bencoding &sourceBuffer)
        {
            if (sourceBuffer.isEmpty())
            {
                throw std::invalid_argument("Empty source buffer passed to be decoded.");
            }
            m_decodeBuffer = sourceBuffer;
        }
        std::byte current() const override
        {
            if (more())
            {
                return (m_decodeBuffer[static_cast<int>(m_bufferPosition)]);
            }
            else
            {
                return (static_cast<std::byte>(EOF));
            }
        }
        void next() override
        {
            if (!more())
            {
                throw std::runtime_error("Decode buffer empty before decode complete.");
            }
            m_bufferPosition++;
        }
        bool more() const override
        {
            return (m_bufferPosition < m_decodeBuffer.size());
        }
        void reset() override
        {
            m_bufferPosition = 0;
        }
    private:
        std::size_t m_bufferPosition = 0;
        Bencoding m_decodeBuffer;
    };
    class FileSource : public ISource
    {
    public:
        FileSource(const std::string &sourceFileName)
        {
            m_source.open(sourceFileName.c_str(), std::ios_base::binary);
            if (!m_source.is_open())
            {
                throw std::runtime_error("Bencode file input stream failed to open or does not exist.");
            }
        }
        std::byte current() const override
        {
            return (static_cast<std::byte>(m_source.peek()));
        }
        void next() override
        {
            char c;
            m_source.get(c);
        }
        bool more() const override
        {
            return (m_source.peek() != EOF);
        }
        void reset() override
        {
            m_source.clear();
            m_source.seekg(0, std::ios_base::beg);
        }
    private:
        mutable std::ifstream m_source;
    };
} // namespace BencodeLib