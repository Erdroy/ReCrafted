// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

// includes
#include "ReCrafted.h"

interface IDisposable
{
public:
    virtual ~IDisposable() = default;
    virtual void dispose() = 0;
};

#endif // IDISPOSABLE_H
