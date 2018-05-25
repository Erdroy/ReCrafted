// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINECOMPONENTBASE_H
#define ENGINECOMPONENTBASE_H

// includes
#include "ReCrafted.h"

/**
* \brief EngineComponentBase base class.
* Implements base class for all engine components with
* virtual functions like: Update, Simulate and render.
*/
class EngineComponentBase
{
    friend class EngineComponentManager;

private:
    void Init()
    {
        OnInit();
    }

protected:
    ~EngineComponentBase() = default;

protected:
    virtual void Shutdown() = 0;

protected:
    virtual void OnInit() = 0;

    virtual void OnLoad()
    {
    }

    virtual void Update()
    {
    }
};

#endif // ENGINECOMPONENTBASE_H
