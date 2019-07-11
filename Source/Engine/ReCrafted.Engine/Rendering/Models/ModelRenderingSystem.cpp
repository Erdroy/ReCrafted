// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ModelRenderingSystem.h"
#include "Core/Transform.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Materials/Material.h"

void ModelRenderingSystem::AllocateModelComponents(const uint32_t amount)
{
    const auto lastRenderListSize = m_renderList.Size();
    m_renderList.Resize(m_renderList.Size() + amount);

    for(auto i = lastRenderListSize; i < lastRenderListSize + amount; i ++)
    {
        const auto& component = &m_renderList[i];
        m_freeComponents.Enqueue(component);
    }
}

void ModelRenderingSystem::Initialize()
{
    m_renderList = {};
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
        // Skip rendering when component is not active, is invalid or out of camera frustum bounds.
        if (!component.Active || !component.IsValid() || !cameraFrustum.Contains(component.Bounds))
            continue;

        const auto& mesh = component.Mesh;
        const auto& material = component.Material;
        const auto& transform = *component.Transform;
        const auto& shader = material->GetShader();

        // Skip if the material doesn't have a shader.
        if (!shader)
            continue;

        // Bind textures
        for(auto i = 0u; i < material->m_textures.Count(); i ++)
            shader->SetTexture(i, material->m_textures[i]);

        // Bind fields
        // TODO: This could be actually improved by building data buffer, then uploading each buffer at once.
        for(auto&& field : material->m_fields)
            Renderer::SetShaderValue(shader->GetHandle(), field->Buffer, field->Id, field->Data, field->Size);

        // TODO: Use model position
        // TODO: Camera-relative rendering.

        // Set shader. This will also update the shader constants.
        if (RenderingManager::GetCurrentShader() != shader)
            RenderingManager::SetCurrentShader(shader, false);

        // Apply shader
        Renderer::ApplyShader(shader->GetHandle(), material->m_shaderPass);

        // Render the mesh
        RenderingManager::DrawIndexedMesh(mesh);
    }

    // Cleanup
    RenderingManager::SetCurrentShader(nullptr);
}

ModelComponent* ModelRenderingSystem::AcquireModelComponent()
{
    ModelComponent* component;

    // This should be getting called only once, but in case of really hungry threads
    // just retry until we will get a free component.
    while (!GetInstance()->m_freeComponents.TryDequeue(component))
        GetInstance()->AllocateModelComponents(DefaultRenderListAdjust);

    // Set component as allocated (i.e. not free).
    component->Free = false;
    return component;
}

void ModelRenderingSystem::ReleaseModelComponent(ModelComponent* component)
{
    ASSERT(component);
    ASSERT(!component->Free);

    // Cleanup the component.
    *component = ModelComponent();

    // Free given component.
    GetInstance()->m_freeComponents.Enqueue(component);
}
