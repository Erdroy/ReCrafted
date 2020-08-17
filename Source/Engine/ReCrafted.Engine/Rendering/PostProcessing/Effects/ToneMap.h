// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Rendering/PostProcessing/PostProcessBase.h"

class ToneMap final : public PostProcessBase
{
private:
    Shader* m_shader = nullptr;

protected:
    ~ToneMap() override;

protected:
    void OnInitialize() override;

    void Render(Shader* shader, 
        const Renderer::Texture2DHandle& colorTexture, 
        const Renderer::Texture2DHandle& normalsTexture, 
        const Renderer::Texture2DHandle& depthTexture, 
        const Renderer::Texture2DHandle& targetTexture) override
    {
        // Do nothing at the moment
        // TODO: ToneMapping
    }

public:
    Shader* GetShader() const override;
};
