// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Singleton.h"
#include "Core/SubSystems/SubSystemBase.h"

template <class TSubSystem>
class SubSystem : public SubSystemBase, public Singleton<TSubSystem>
{
    DELETE_COPY_MOVE(SubSystem)

protected:
    SubSystem() = default;
    virtual ~SubSystem() = default;

protected:
    void Release() override
    {
        Singleton<TSubSystem>::Dispose();
    }
};
