// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class RenderingComponentBase
{
    friend class RenderingManager;
    DELETE_COPY_MOVE(RenderingComponentBase)

protected:
    RenderingComponentBase() = default;
    virtual ~RenderingComponentBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

protected:
    virtual void Release() = 0;
};