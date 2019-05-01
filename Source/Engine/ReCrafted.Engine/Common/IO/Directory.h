// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class Directory
{
    DELETE_COPY_MOVE(Directory)

public:
    static bool Exists(const char* path);
    static bool Create(const char* path);
};
