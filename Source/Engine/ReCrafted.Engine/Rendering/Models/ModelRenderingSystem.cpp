// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ModelRenderingSystem.h"
#include "Core/Transform.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Materials/Material.h"

void ModelRenderingSystem::Initialize()
{
    m_renderList = {};
    m_renderList.Resize(DefaultRenderListLength);

    for(auto&& component : m_renderList)
        m_freeComponents.Enqueue(&component);
}

void ModelRenderingSystem::Shutdown()
{
    m_renderList.Release();
}

void ModelRenderingSystem::Render()
{
    auto cameraFrustum = Camera::GetMainCamera()->GetBoundingFrustum();

    // TODO: render list sorting

    for (auto&& component : m_renderList)
    {
        // Skip rendering when component is not active, doesn't have material or mesh.
        if (!component.Active || component.Material == nullptr || component.Mesh == nullptr)
            return;

        // Skip rendering when mesh is out of camera's bounds
        if (!cameraFrustum.Contains(component.Bounds))
            continue;

        const auto& mesh = component.Mesh;
        const auto& material = component.Material;
        const auto& transform = *component.Transform;
        const auto& shader = material->GetShader();

        // TODO: Update shader values

        // TODO: Camera-relative rendering.

        // Set shader. This will also update the shader constants.
        if (RenderingManager::GetCurrentShader() != shader)
            RenderingManager::SetCurrentShader(shader, false);

        // Render the mesh
        RenderingManager::DrawIndexedMesh(mesh);
    }

    // Cleanup
    RenderingManager::SetCurrentShader(nullptr);
}
