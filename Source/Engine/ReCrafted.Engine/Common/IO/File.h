// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

struct File
{
private:
    FILE* m_file = nullptr;
    size_t m_fileSize = 0u;

public:
    enum class OpenMode
    {
        BinaryRead,
        BinaryWrite,
        BinaryAppend,
        BinaryReadWrite
    };

public:
    File(FILE* handle, const size_t size) : m_file(handle), m_fileSize(size) { }

public:
    void Seek(long position) const;
    void Read(void* buffer, size_t length, size_t offset = 0) const;
    void Read(void* buffer) const;
    void Write(void* data, size_t dataLength) const;
    void Flush() const;
    void Close() const;

public:
    size_t Size() const
    {
        return m_fileSize;
    }

    bool IsValid() const
    {
        return m_file != nullptr;
    }

public:
    static File Open(const char* fileName, OpenMode openMode);
    static bool Exists(const char* fileName);
    static void Delete(const char* fileName);
    static void Move(const char* sourceFileName, const char* destinationFileName);
    static void Copy(const char* sourceFileName, const char* destinationFileName);
};
