// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Common/IO/Directory.h"

#ifdef _WIN32
#include <Windows.h>

bool Directory::Exists(const char* path)
{
    const auto fType = GetFileAttributesA(path);

    if (fType == INVALID_FILE_ATTRIBUTES)
        return false;

    return fType & FILE_ATTRIBUTE_DIRECTORY;
}

bool Directory::Create(const char* path)
{
    return CreateDirectoryA(path, nullptr);
}
#endif
