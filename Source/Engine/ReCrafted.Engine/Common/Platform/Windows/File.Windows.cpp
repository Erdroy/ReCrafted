// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Common/IO/File.h"

#ifdef _WIN32

#include <Windows.h>

void File::Seek(const long position) const
{
    fseek(m_file, position, SEEK_SET);
}

void File::Read(void* buffer, const size_t length, const size_t offset) const
{
    ASSERT(m_file);

    if (offset > 0)
    {
        // set offset
        fseek(m_file, long(offset), SEEK_SET);
    }

    // read
    fread(buffer, 1, length, m_file);
}

void File::Read(void* buffer) const
{
    Read(buffer, m_fileSize);
}

void File::Write(void* data, const size_t dataLength) const
{
    fwrite(data, dataLength, 1, m_file);
}

void File::Flush() const
{
    fflush(m_file);
}

void File::Close() const
{
    fclose(m_file);
}

File File::Open(const char* fileName, const OpenMode openMode)
{
    // select open mode
    const char* mode = nullptr;
    switch (openMode)
    {
    case OpenMode::BinaryRead:
        mode = "rb";
        break;
    case OpenMode::BinaryWrite:
        mode = "wb";
        break;
    case OpenMode::BinaryAppend:
        mode = "wb+";
        break;
    case OpenMode::BinaryReadWrite:
        mode = "rb+";
        break;
    default:
        return { nullptr, 0u };
    }

    const auto fileHandle = fopen(fileName, mode);
    ASSERT(fileHandle);

    // measure file size
    fseek(fileHandle, 0, SEEK_END);
    const auto fileSize = ftell(fileHandle);
    fseek(fileHandle, 0, SEEK_SET);

    return File(fileHandle, fileSize);
}

bool File::Exists(const char* fileName)
{
    FILE* file;
    if (fopen_s(&file, fileName, "rb") == 0)
    {
        fclose(file);
        return true;
    }

    return false;
}

void File::Delete(const char* fileName)
{
    ASSERT(DeleteFileA(fileName));
}

void File::Move(const char* sourceFileName, const char* destinationFileName)
{
    ASSERT(MoveFileA(sourceFileName, destinationFileName));
}

void File::Copy(const char* sourceFileName, const char* destinationFileName)
{
    ASSERT(CopyFileA(sourceFileName, destinationFileName, TRUE));
}
#endif
