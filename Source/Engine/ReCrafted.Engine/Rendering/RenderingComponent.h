// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Singleton.h"
#include "Rendering/RenderingComponentBase.h"

template <class TRenderingComponent>
class RenderingComponent : public RenderingComponentBase, public Singleton<TRenderingComponent>
{
    friend class RenderingManager;
    DELETE_COPY_MOVE(RenderingComponent)

protected:
    RenderingComponent() = default;
    virtual ~RenderingComponent() = default;

protected:
    void Release() override
    {
        Singleton<TRenderingComponent>::Dispose();
    }
};
