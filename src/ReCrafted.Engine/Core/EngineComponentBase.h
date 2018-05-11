// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINECOMPONENTBASE_H
#define ENGINECOMPONENTBASE_H

// includes
#include "ReCrafted.h"

/**
* \brief EngineComponentBase base class.
* Implements base class for all engine components with
* virtual functions like: update, simulate and render.
*/
class EngineComponentBase
{
    friend class EngineComponentManager;

private:
    void init()
    {
        onInit();
    }

protected:
    ~EngineComponentBase() = default;

protected:
    virtual void shutdown() = 0;

protected:
    virtual void onInit() = 0;

    virtual void onLoad()
    {
    }

    virtual void update()
    {
    }
};

#endif // ENGINECOMPONENTBASE_H
