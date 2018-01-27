// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef FILESTREAM_H
#define FILESTREAM_H

// include
#include "Stream.h"
#include "Platform/Platform.h"

class FileStream : public Stream
{
private:
    File m_file = {};

public:
    /**
    * \brief Opens FileStream for writing or/and reading data.
    * \param fileName The file to read from or write to.
    * \param openMode The file open mode.
    */
    explicit FileStream(const char* fileName, const OpenMode::_enum openMode)
    {
        Platform::openFile(&m_file, fileName, openMode);
    }

public:
    /**
    * \brief Reads data from the buffer with given offset and of given count.
    * \param buffer The buffer to read from.
    * \param offset The read offset in bytes.
    * \param count The count of bytes to read.
    * \return The count of read bytes.
    */
    size_t read(void* buffer, const size_t offset, const size_t count) const override
    {
        return Stream::read(buffer, offset, count);
    }

    /**
    * \brief Writes data to the buffer with given offset and of given count.
    * \param buffer The buffer to write to.
    * \param offset The write offset in bytes.
    * \param count The count of bytes to write.
    */
    void write(void* buffer, const size_t offset, const size_t count) const override
    {
        Stream::write(buffer, offset, count);
    }

    /**
    * \brief Reads data from the buffer with given offset and of given count.
    * \param buffer The buffer to read from.
    * \param bufferSize The buffer size.
    * \param offset The read offset in bytes.
    * \param count The count of bytes to read.
    * \return The count of read bytes.
    */
    size_t read(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const override
    {
        // calculate the size of buffer with offset and
        // check if we can read to the buffer anything after the offset being applied
        cvar outputOffsetSize = bufferSize - offset;
        assert(outputOffsetSize > 0);

        // read from the file
        m_file.read(buffer, count, offset);

        // done
        return count;
    }

    /**
    * \brief Writes data to the buffer with given offset and of given count.
    * \param buffer The buffer to write to.
    * \param bufferSize The buffer size.
    * \param offset The write offset in bytes.
    * \param count The count of bytes to write.
    */
    void write(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const override
    {
        // calculate the size of buffer with offset and
        // check if we can write to the buffer anything after the offset being applied
        cvar outputOffsetSize = bufferSize - offset;
        assert(outputOffsetSize > 0);

        // seek position
        m_file.seek(static_cast<long>(offset));

        // write to the file
        m_file.write(buffer, count);
    }

    /**
    * \brief Flushes the file stream.
    */
    void flush() const
    {
        m_file.flush();
    }

protected:
    /**
    * \brief Closes the stream.
    */
    void close() const override
    {
        m_file.flush();
        m_file.close();
    }
};

#endif // FILESTREAM_H
