// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/**
 * \brief Stream base class. 
 * Implements basic data stream.
 */
class Stream
{
private:
    byte* m_buffer = nullptr;
    size_t m_bufferSize = 0u;

protected:
    bool m_open = false;

protected:
    Stream()
    {
    }

public:
    virtual ~Stream() = default;

    /**
     * \brief Opens Stream for basic read/write usage.
     * \param buffer The buffer to read or write to.
     * \param bufferSize The size of the buffer. Must be > 0.
     */
    Stream(void* buffer, const size_t bufferSize)
    {
        ASSERT(bufferSize > 0);

        m_open = true;
        m_buffer = static_cast<byte*>(buffer);
        m_bufferSize = bufferSize;
    }

public:
    /**
     * \brief Returns stream open state.
     * \return True when this stream is open.
     */
    bool IsOpen() const
    {
        return m_open;
    }

    /**
     * \brief Disposes the stream.
     */
    void Dispose()
    {
        if (m_open)
        {
            Close();
            m_open = false;
        }
    }

public:
    /**
    * \brief Reads data from the buffer with given offset and of given count.
    * \param buffer The buffer to read from.
    * \param offset The read offset in bytes.
    * \param count The count of bytes to read.
    * \return The count of read bytes.
    */
    virtual size_t Read(void* buffer, const size_t offset, const size_t count) const
    {
        return Read(buffer, count, offset, count);
    }

    /**
    * \brief Writes data to the buffer with given offset and of given count.
    * \param buffer The buffer to write to.
    * \param offset The write offset in bytes.
    * \param count The count of bytes to write.
    */
    virtual void Write(void* buffer, const size_t offset, const size_t count) const
    {
        Write(buffer, count, offset, count);
    }

    /**
     * \brief Reads data from the buffer with given offset and of given count.
     * \param buffer The buffer to read from.
     * \param bufferSize The buffer size.
     * \param offset The read offset in bytes.
     * \param count The count of bytes to read.
     * \return The count of read bytes.
     */
    virtual size_t Read(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const
    {
        // check if the stream is open
        ASSERT(m_open);

        // do you really want to read no data? there is no any sense :v
        ASSERT(count > 0);

        // calculate the size of buffer with offset and
        // check if we can read to the buffer anything after the offset being applied
        const auto outputOffsetSize = bufferSize - offset;
        ASSERT(outputOffsetSize >= 0);

        // calculate the size of buffer with offset and
        // check if we can read anything after the offset being applied
        const auto bufferOffsetSize = m_bufferSize - offset;
        ASSERT(bufferOffsetSize > 0);

        // calculate the amout of bytes to read (clamp to the size of buffer)
        const auto bytesToRead = outputOffsetSize < count ? outputOffsetSize : count;

        // copy data
        memcpy(buffer, m_buffer + offset, bytesToRead);

        // done
        return bytesToRead;
    }

    /**
    * \brief Writes data to the buffer with given offset and of given count.
    * \param buffer The buffer to write to.
    * \param bufferSize The buffer size.
    * \param offset The write offset in bytes.
    * \param count The count of bytes to write.
    */
    virtual void Write(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const
    {
        // check if the stream is open
        ASSERT(m_open);

        // do you really want to write no data? there is no any sense :v
        ASSERT(count > 0);

        // calculate the size of buffer with offset and
        // check if we can write to the buffer anything after the offset being applied
        const auto outputOffsetSize = bufferSize - offset;
        ASSERT(outputOffsetSize > 0);

        // calculate the size of buffer with offset and
        // check if we can write anything after the offset being applied
        const auto bufferOffsetSize = m_bufferSize - offset;
        ASSERT(bufferOffsetSize > 0);

        // calculate the amout of bytes to read (clamp to the size of buffer)
        const auto bytesToWrite = bufferOffsetSize < count ? bufferOffsetSize : count;

        // copy data
        memcpy(m_buffer + offset, buffer, bytesToWrite);
    }

protected:
    /**
     * \brief Closes the stream.
     */
    virtual void Close() const
    {
    }
};
