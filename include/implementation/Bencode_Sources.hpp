#pragma once
//
// C++ STL
//
#include <fstream>
#include <string>
#include <vector>
//
// ISource interface
//
#include "ISource.hpp"
// =========
// NAMESPACE
// =========
namespace BencodeLib
{
    // ==================================
    // Source classes for Bencode decoder
    // ==================================
    //
    // Buffered character source
    //
    class BufferSource : public ISource
    {
    public:
        explicit BufferSource(const std::string &sourceBuffer)
        {
            if (sourceBuffer.empty())
            {
                throw Error("Empty source buffer passed to be decoded.");
            }
            for (auto ch : sourceBuffer)
            {
                m_decodeBuffer.push_back(static_cast<std::byte>(ch));
            }
        }
        [[nodiscard]] char current() const override
        {
            if (more())
            {
                return (static_cast<char>(m_decodeBuffer[static_cast<int>(m_bufferPosition)]));
            }
            return (static_cast<char>(EOF));
        }
        void next() override
        {
            if (!more())
            {
                throw Error("Decode buffer empty before decode complete.");
            }
            m_bufferPosition++;
        }
        [[nodiscard]] bool more() const override
        {
            return (m_bufferPosition < m_decodeBuffer.size());
        }
        void reset() override
        {
            m_bufferPosition = 0;
        }

    private:
        std::size_t m_bufferPosition = 0;
        std::vector<std::byte> m_decodeBuffer;
    };
    //
    // Buffered character source
    //
    class FileSource : public ISource
    {
    public:
        explicit FileSource(const std::string &sourceFileName)
        {
            m_source.open(sourceFileName.c_str(), std::ios_base::binary);
            if (!m_source.is_open())
            {
                throw Error("Bencode file input stream failed to open or does not exist.");
            }
        }
        char current() const override
        {
            return (static_cast<char>(m_source.peek()));
        }
        void next() override
        {
            char c = 0;
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