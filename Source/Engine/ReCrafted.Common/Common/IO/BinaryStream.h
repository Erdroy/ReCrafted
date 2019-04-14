// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Stream.h"

/**
 * \brief BinaryStream class. Implements reading and writing binary data to stream.
 */
class BinaryStream final : public Stream
{
#define DEFINE_READER(type, name)                   \
    type Read##name(){                              \
        type tmp = type();                          \
        m_stream.Read(&tmp, sizeof(type),      \
        m_position, sizeof(type));                  \
        m_position += sizeof(type);                 \
        return tmp;                                 \
    }

#define DEFINE_WRITER(type, name)                   \
    void Write##name(type value){                   \
        m_stream.Write(&value, sizeof(type),   \
        m_position, sizeof(type));                  \
        m_position += sizeof(type);                 \
    }

#define DEFINE_TYPE(type, name)                     \
    DEFINE_READER(type, name)                       \
    DEFINE_WRITER(type, name)

private:
    Stream& m_stream;
    size_t m_position = 0u;

public:
    /**
     * \brief Opens BinaryStream for writing or/and reading binary data.
     * \param stream The stream to read from or write to.
     */
    explicit BinaryStream(Stream& stream): m_stream(stream)
    {
        m_open = stream.IsOpen();
    }

public:
    /**
    * \brief Reads Byte.
    */
    template<typename T>
    DEFINE_READER(T, Struct)

    /**
    * \brief Writes Byte.
    */
    template<typename T>
    DEFINE_WRITER(T, Struct)

    /**
    * \brief Reads Byte.
    */
    DEFINE_READER(byte, Byte)

    /**
     * \brief Writes Byte.
     */
    DEFINE_WRITER(byte, Byte)

    /**
    * \brief Reads Bool.
    */
    DEFINE_READER(bool, Bool)

    /**
     * \brief Writes Bool.
     */
    DEFINE_WRITER(bool, Bool)

    /**
    * \brief Reads Int.
    */
    DEFINE_READER(int, Int)

    /**
     * \brief Writes Int.
     */
    DEFINE_WRITER(int, Int)

    /**
    * \brief Reads Single.
    */
    DEFINE_READER(float, Single)

    /**
     * \brief Writes Single.
     */
    DEFINE_WRITER(float, Single)

    /**
    * \brief Reads Double.
    */
    DEFINE_READER(double, Double)

    /**
     * \brief Writes Double.
     */
    DEFINE_WRITER(double, Double)

    /**
    * \brief Reads byUInt16te.
    */
    DEFINE_READER(uint16_t, UInt16)

    /**
     * \brief Writes UInt16.
     */
    DEFINE_WRITER(uint16_t, UInt16)

    /**
    * \brief Reads UInt32.
    */
    DEFINE_READER(uint32_t, UInt32)

    /**
     * \brief Writes UInt32.
     */
    DEFINE_WRITER(uint32_t, UInt32)

    /**
    * \brief Reads UInt64.
    */
    DEFINE_READER(uint64_t, UInt64)

    /**
     * \brief Writes UInt64.
     */
    DEFINE_WRITER(uint64_t, UInt64)

    /**
    * \brief Reads Int16.
    */
    DEFINE_READER(int16_t, Int16)

    /**
     * \brief Writes Int16.
     */
    DEFINE_WRITER(int16_t, Int16)

    /**
    * \brief Reads Int32.
    */
    DEFINE_READER(int32_t, Int32)

    /**
     * \brief Writes Int32.
     */
    DEFINE_WRITER(int32_t, Int32)

    /**
    * \brief Reads Int64.
    */
    DEFINE_READER(int64_t, Int64)

    /**
     * \brief Writes Int64.
     */
    DEFINE_WRITER(int64_t, Int64)

public:
    /**
     * \brief Reads bytes from this stream.
     * \param buffer The buffer to read to.
     * \param count The amount of bytes to read.
     */
    void ReadBytes(char* buffer, const size_t count)
    {
        const auto read = m_stream.Read(buffer, count, m_position, count);
        
        ASSERT(read == count);

        m_position += count;
    }

    /**
    * \brief Writes bytes to this stream.
    * \param buffer The buffer to read from.
    * \param count The amount of bytes to read.
    */
    void WriteBytes(char* buffer, const size_t count)
    {
        m_stream.Write(buffer, count, m_position, count);
        m_position += count;
    }

    /**
     * \brief Reads ANSI string to given buffer.
     * \param string The string buffer.
     */
    void ReadString(char* string)
    {
        const auto length = ReadUInt16();
        ReadBytes(string, length);
    }

    /**
     * \brief Writes string to this stream.
     * \param string The ANSI string.
     */
    void WriteString(char* string)
    {
        const auto length = strlen(string);
        WriteUInt16(static_cast<uint16_t>(length));
        WriteBytes(string, length);
    }

    /**
    * \brief Writes string to this stream. 
    * \param string The ANSI string.
    * 
    * \note Data won't get a Length prefix!
    */
    void WriteStringRaw(char* string)
    {
        const auto length = strlen(string);
        WriteBytes(string, length);
    }

    /**
     * \brief Resets the position of the stream.
     */
    void Reset()
    {
        m_position = 0u;
    }

public:
    /**
     * \brief Gets the read/write position.
     * \return The read/write position.
     */
    size_t Position() const
    {
        return m_position;
    }

    /**
     * \brief Sets the read/write position.
     * \return The read/write position.
     */
    void Position(const size_t position)
    {
        m_position = position;
    }

protected:
    /**
    * \brief Closes the stream.
    */
    void Close() const override
    {
        m_stream.Dispose();
    }

#undef DEFINE_READER
#undef DEFINE_WRITER
#undef DEFINE_TYPE
};

