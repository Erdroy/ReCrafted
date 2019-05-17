// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Rendering/RenderingBase.h"
#include "Rendering/RenderingManager.h"

class DeferredRendering final : public RenderingBase
{
    DELETE_COPY_MOVE(DeferredRendering)

private:
    RefPtr<RenderBuffer> m_gbuffer = nullptr;
    Shader* m_gbufferFillShader = nullptr;
    Shader* m_gbufferCombine = nullptr;

public:
    DeferredRendering() = default;
    ~DeferredRendering() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

    void BeginRender() override;
    void EndRender() override;
};