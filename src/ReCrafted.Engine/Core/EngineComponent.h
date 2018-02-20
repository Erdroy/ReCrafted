// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINECOMPONENT_H
#define ENGINECOMPONENT_H

// includes
#include "ReCrafted.h"
#include "Core/IDisposable.h"

/**
* \brief EngineComponent base class.
* Implements base class for all engine components with
* virtual functions like: update, simulate and render.
*/
class EngineComponent : public IDisposable
{
    friend class EngineComponentManager;

private:
    void init()
    {
        onInit();
    }

    void dispose() override
    {
        // call on shutown as we are closing now
        onShutdown();
    }

protected:
    ~EngineComponent() = default;

protected:
    virtual void onInit() = 0;
    virtual void onShutdown() = 0;

    virtual void onLoad() {}
};

#endif // ENGINECOMPONENT_H
