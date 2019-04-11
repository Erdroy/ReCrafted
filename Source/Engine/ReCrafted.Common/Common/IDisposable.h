// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

struct IDisposable
{
public:
    virtual ~IDisposable() = default;
    virtual void Dispose() = 0;
};

#endif // IDISPOSABLE_H
