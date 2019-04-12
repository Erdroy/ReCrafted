// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class SubSystemBase
{
    friend class SubSystemManager;
    DELETE_COPY_MOVE(SubSystemBase)

protected:
    SubSystemBase() = default;
    ~SubSystemBase() = default;

protected:
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

protected:
    virtual void OnUpdate() { }
    virtual void OnLateUpdate() { }
    virtual void OnFrameDone() { }
};
