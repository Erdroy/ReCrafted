// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Common/Lock.h"
#include "Common/Queue.h"
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
    Queue<ModelComponent*> m_freeComponents;

    Lock m_mainRenderLock;

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
    ///     Gets the main lock of the model rendering system.
    /// </summary>
    /// <returns>The main lock pointer.</returns>
    static Lock* GetMainLock();

public:
    /// <summary>
    ///     Acquires free model component, which is initially disabled.
    /// </summary>
    /// <returns>The acquired model component.</returns>
    /// <remarks>
    ///     Acquired model component should be modified only from the main thread!
    ///     Optionally you should use component->BeginModify(), then modify and at the end call component->EndModify(),
    ///     but this is slow, as it locks the main thread from model rendering or edit during the rendering.
    /// </remarks>
    /// <remarks>Can be called only from the main thread.</remarks>
    static ModelComponent* AcquireModelComponent(bool isProcedural = false);

    /// <summary>
    ///     Releases given component.
    /// </summary>
    /// <param name="component">The component to return to the pool.</param>
    /// <remarks>Can be called only from the main thread.</remarks>
    static void ReleaseModelComponent(ModelComponent* component);
};
