// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Stream.h"
#include "Common/IO/File.h"

class FileStream final : public Stream
{
private:
    File m_file = { nullptr, 0u };
    bool m_autoClose = true;

private:
    FileStream() = default;

public:
    /**
    * \brief Opens FileStream for writing or/and reading data.
    * \param fileName The file to read from or write to.
    * \param openMode The file open mode.
    */
    explicit FileStream(const char* fileName, const File::OpenMode openMode)
    {
        m_file = File::Open(fileName, openMode);
        m_open = true;
        m_autoClose = true;
    }

    /**
     * \brief Opens FileStream from given file.
     * \param file The file handle.
     * 
     * \note When this stream closes, only file flush will be called. 
     * You need to manually call file.Close()
     */
    explicit FileStream(const File& file) : m_file(file)
    {
        m_open = true;
        m_autoClose = false;
    }

public:
    /**
    * \brief Reads data from the buffer with given offset and of given count.
    * \param buffer The buffer to read from.
    * \param offset The read offset in bytes.
    * \param count The count of bytes to read.
    * \return The count of read bytes.
    */
    size_t Read(void* buffer, const size_t offset, const size_t count) const override
    {
        return Stream::Read(buffer, offset, count);
    }

    /**
    * \brief Writes data to the buffer with given offset and of given count.
    * \param buffer The buffer to write to.
    * \param offset The write offset in bytes.
    * \param count The count of bytes to write.
    */
    void Write(void* buffer, const size_t offset, const size_t count) const override
    {
        Stream::Write(buffer, offset, count);
    }

    /**
    * \brief Reads data from the buffer with given offset and of given count.
    * \param buffer The buffer to read from.
    * \param bufferSize The buffer size.
    * \param offset The read offset in bytes.
    * \param count The count of bytes to read.
    * \return The count of read bytes.
    */
    size_t Read(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const override
    {
        // calculate the size of buffer with offset and
        // check if we can read to the buffer anything after the offset being applied
        const auto outputOffsetSize = bufferSize - offset;
        ASSERT(outputOffsetSize >= 0);

        // read from the file
        m_file.Read(buffer, count, offset);

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
    void Write(void* buffer, const size_t bufferSize, const size_t offset, const size_t count) const override
    {
        // calculate the size of buffer with offset and
        // check if we can write to the buffer anything after the offset being applied
        const auto outputOffsetSize = bufferSize - offset;
        ASSERT(outputOffsetSize >= 0);

        // seek position
        m_file.Seek(static_cast<long>(offset));

        // write to the file
        m_file.Write(buffer, count);
    }

    /**
    * \brief Flushes the file stream.
    */
    void Flush() const
    {
        m_file.Flush();
    }

protected:
    /**
    * \brief Closes the stream.
    * 
    * \remarks To close the stream, please use Dispose function.
    */
    void Close() const override
    {
        m_file.Flush();

        if(m_autoClose)
            m_file.Close();
    }
};
