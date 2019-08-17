// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ModelRenderingSystem.h"
#include "Core/Transform.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Materials/Material.h"
#include "Profiler/Profiler.h"
#include "Rendering/Debug/DebugDraw.h"

void ModelRenderingSystem::AllocateModelComponents(const uint32_t amount)
{
    for(auto i = 0; i < amount; i ++)
    {
        const auto& component = new ModelComponent();
        m_renderList.Add(component);
        m_freeComponents.Enqueue(component);
    }
}

void ModelRenderingSystem::Initialize()
{
    m_renderList = {};
    AllocateModelComponents(256);
}

void ModelRenderingSystem::Shutdown()
{
    for(auto&& component : m_renderList)
        delete component;
    m_renderList.Release();
}

void ModelRenderingSystem::Render()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    auto cameraFrustum = Camera::GetMainCamera()->GetBoundingFrustum();

    // TODO: render list sorting
    for (auto&& component : m_renderList)
    {
        // Skip rendering when component is not active, is invalid or out of camera frustum bounds.
        if (!component->Active || !component->IsValid() || !cameraFrustum.IntersectsSphere(component->Bounds))
            continue;

        // Render all of the meshes
        auto meshIdx = 0;
        for (auto& mesh : component->Meshes)
        {
            // Invoke OnBeginRender on procedural models
            if (component->Procedural)
                component->OnBeginRender.Invoke(meshIdx);

            const auto& material = component->Material;
            const auto& transform = *component->Transform;
            const auto& shader = material->GetShader();

            // Skip if the material doesn't have a shader.
            if (!shader)
                continue;

            // Update shader fields values
            for (auto&& field : material->m_fields)
                Renderer::SetShaderValue(shader->GetHandle(), field->Buffer, field->Id, field->Data, field->Size);

            // TODO: Use model position
            // TODO: Camera-relative rendering.

            // Set shader. This will also update the shader constants.
            if (RenderingManager::GetCurrentShader() != shader)
                RenderingManager::SetCurrentShader(shader, false);

            // Apply shader
            Renderer::ApplyShader(shader->GetHandle(), material->m_shaderPass);

            // Bind texture arrays
            for (auto i = 0u; i < material->m_textureArrays.Count(); i++)
            {
                auto& textureArray = material->m_textureArrays[i];
                shader->SetTextureArray(i, textureArray.ToArray());
            }

            // Bind textures
            for (auto i = 0u; i < material->m_textures.Count(); i++)
                shader->SetTexture(i, material->m_textures[i]);

            // Render current mesh
            RenderingManager::DrawIndexedMesh(mesh);
            meshIdx++;
        }
    }

    // Cleanup
    RenderingManager::SetCurrentShader(nullptr);
}

ModelComponent* ModelRenderingSystem::AcquireModelComponent(const bool isProcedural)
{
    ModelComponent* component;

    // This should be getting called only once, but in case of really hungry threads
    // just retry until we will get a free component.
    while (!GetInstance()->m_freeComponents.TryDequeue(component))
        GetInstance()->AllocateModelComponents(DefaultRenderListAdjust);

    // Set component as allocated (i.e. not free).
    component->Free = false;
    component->Procedural = isProcedural;
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
