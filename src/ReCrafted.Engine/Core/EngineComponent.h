// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINECOMPONENT_H
#define ENGINECOMPONENT_H

// includes
#include "ReCrafted.h"
#include "Singleton.h"
#include "EngineComponentBase.h"

template <class T>
class EngineComponent : public EngineComponentBase, public Singleton<T>
{
    friend class EngineComponentManager;

protected:
    ~EngineComponent() = default;

private:
    void Shutdown() override
    {
        Singleton<T>::Dispose();
    }
};

#endif // ENGINECOMPONENT_H
