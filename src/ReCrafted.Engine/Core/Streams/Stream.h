// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef STREAM_H
#define STREAM_H

// includes
#include "Core/IDisposable.h"

/**
 * \brief Stream base class. 
 * Implements basic data stream.
 */
class Stream : IDisposable
{
private:
    bool m_open = false;

    byte* m_buffer = nullptr;
    size_t m_bufferSize = 0u;

protected:
    Stream() { }

public:
    /**
     * \brief Opens Stream for basic read/write usage.
     * \param buffer The buffer to read or write to.
     * \param bufferSize The size of the buffer. Must be > 0.
     */
    Stream(void* buffer, const size_t bufferSize)
    {
        assert(bufferSize > 0);

        m_open = true;
        m_buffer = static_cast<byte*>(buffer);
        m_bufferSize = bufferSize;
    }

public:
    /**
     * \brief Returns stream open state.
     * \return True when this stream is open.
     */
    bool isOpen() const
    {
        return m_open;
    }

    /**
     * \brief Disposes the stream.
     */
    void dispose() override
    {
        if (m_open)
        {
            close();
            m_open = false;
        }
    }

public:
    /**
     * \brief Reads data from the buffer with given offset and of given count.
     * \param buffer The buffer to read from.
     * \param bufferSize The buffer size.
     * \param offset The read offset in bytes.
     * \param count The count of bytes to read.
     * \return The count of read bytes.
     */
    virtual size_t read(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const
    {
        // check if the stream is open
        assert(m_open);

        // do you really want to read no data? there is no any sense :v
        assert(count > 0);

        // calculate the size of buffer with offset and
        // check if we can read to the buffer anything after the offset being applied
        cvar outputOffsetSize = bufferSize - offset;
        assert(outputOffsetSize > 0);

        // calculate the size of buffer with offset and
        // check if we can read anything after the offset being applied
        cvar bufferOffsetSize = m_bufferSize - offset;
        assert(bufferOffsetSize > 0);

        // calculate the amout of bytes to read (clamp to the size of buffer)
        cvar bytesToRead = outputOffsetSize < count ? outputOffsetSize : count;

        // copy data
        memcpy(static_cast<byte*>(buffer) + offset, m_buffer, bytesToRead);

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
    virtual void write(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const
    {
        // check if the stream is open
        assert(m_open);

        // do you really want to write no data? there is no any sense :v
        assert(count > 0);

        // calculate the size of buffer with offset and
        // check if we can write to the buffer anything after the offset being applied
        cvar outputOffsetSize = bufferSize - offset;
        assert(outputOffsetSize > 0);

        // calculate the size of buffer with offset and
        // check if we can write anything after the offset being applied
        cvar bufferOffsetSize = m_bufferSize - offset;
        assert(bufferOffsetSize > 0);

        // calculate the amout of bytes to read (clamp to the size of buffer)
        cvar bytesToWrite = bufferOffsetSize < count ? bufferOffsetSize : count;

        // copy data
        memcpy(m_buffer + offset, buffer, bytesToWrite);
    }

protected:
    /**
     * \brief Closes the stream.
     */
    virtual void close() {};
};

#endif // STREAM_H
