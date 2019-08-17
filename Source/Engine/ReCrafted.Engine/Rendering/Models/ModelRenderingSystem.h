// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Common/ConcurrentQueue.h"
#include "Rendering/RenderingComponent.h"
#include "Rendering/Models/ModelComponent.h"

class ModelRenderingSystem final : public RenderingComponent<ModelRenderingSystem>
{
    friend class RenderingManager;
public:
    /// <summary>
    ///     The amount of model components that will be added to the list, 
    ///     if we run out of empty ones.
    /// </summary>
    static const uint32_t DefaultRenderListAdjust = 16;

private:
    List<ModelComponent*> m_renderList;
    ConcurrentQueue<ModelComponent*> m_freeComponents;

private:
    void AllocateModelComponents(uint32_t amount);

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

public:
    /// <summary>
    ///     Acquires free model component, which is initially disabled.
    /// </summary>
    /// <returns>The acquired model component.</returns>
    static ModelComponent* AcquireModelComponent(bool isProcedural = false);

    /// <summary>
    ///     Releases given component.
    /// </summary>
    /// <param name="component"></param>
    static void ReleaseModelComponent(ModelComponent* component);
};
