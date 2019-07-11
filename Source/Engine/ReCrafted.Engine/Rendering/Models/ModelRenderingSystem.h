// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Common/ConcurrentQueue.h"
#include "Rendering/RenderingComponent.h"
#include "Rendering/Models/ModelComponent.h"

class ModelRenderingSystem : public RenderingComponent<ModelRenderingSystem>
{
    friend class RenderingManager;
public:
    /// <summary>
    ///     The default amount of model components reserved at startup.
    /// </summary>
    static const uint32_t DefaultRenderListLength = 256;

private:
    List<ModelComponent> m_renderList;
    ConcurrentQueue<ModelComponent*> m_freeComponents;

public:
    ModelRenderingSystem() = default;
    ~ModelRenderingSystem() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

    void Render() override;

public:
    RenderingComponentStage GetStage() override
    {
        return RenderingComponentStage::Geometry;
    }
};
