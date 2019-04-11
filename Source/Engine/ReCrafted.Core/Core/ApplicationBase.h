// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef APPLICATIONBASE_H
#define APPLICATIONBASE_H

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

#endif // APPLICATIONBASE_H
