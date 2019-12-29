// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Rendering/Shader.h"
#include "Rendering/Renderer/Renderer.h"

class PostProcessBase
{
    friend class PostProcessingManager;
    DELETE_COPY_MOVE(PostProcessBase)

public:
    PostProcessBase() = default;

protected:
    virtual ~PostProcessBase() = default;

protected:
    virtual void OnInitialize() = 0;
    virtual void Render(Shader* shader, 
        const Renderer::Texture2DHandle& colorTexture, 
        const Renderer::Texture2DHandle& normalsTexture, 
        const Renderer::Texture2DHandle& depthTexture,
        const Renderer::Texture2DHandle& targetTexture)
    {
        // Set effect's shader as the current one
        RenderingManager::SetCurrentShader(shader, true);

        // Blit
        Renderer::Texture2DHandle textures[3] = { colorTexture, normalsTexture, depthTexture };
        Renderer::BlitTextures(targetTexture, textures, 3, shader->GetHandle());
    }

public:
    virtual Shader* GetShader() const = 0;
};