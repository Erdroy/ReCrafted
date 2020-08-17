// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/**
* \brief Application base class.
*/
class ApplicationBase
{
protected:
    ~ApplicationBase() = default;

public:
    virtual void Run() = 0;
    virtual void Shutdown() = 0;
};
