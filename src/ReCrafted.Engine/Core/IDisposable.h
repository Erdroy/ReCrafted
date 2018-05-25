// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

// includes
#include "ReCrafted.h"
#include "Defines.h"

struct IDisposable
{
public:
    virtual ~IDisposable() = default;
    virtual void Dispose() = 0;
};

#endif // IDISPOSABLE_H
