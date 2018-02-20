// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

class IDisposable
{
public:
    virtual ~IDisposable() = default;
    virtual void dispose() = 0;
};

#endif // IDISPOSABLE_H
