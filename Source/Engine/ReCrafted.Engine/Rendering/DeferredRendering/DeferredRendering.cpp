// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DeferredRendering.h"
#include "Common/Logger.h"
#include "Content/ContentManager.h"
#include "Rendering/Shader.h"

void DeferredRendering::Initialize()
{
    Logger::Log("Initializing deferred rendering system");

    Logger::Log("Loading deferred rendering shaders");
    m_gbufferFillShader = ContentManager::LoadAsset<Shader>("Shaders/StandardShader");
    ASSERT(m_gbufferFillShader);

    m_gbufferCombine = ContentManager::LoadAsset<Shader>("Shaders/GBufferCombine");
    ASSERT(m_gbufferCombine);

    Logger::Log("Deferred rendering initialized");
}

void DeferredRendering::Shutdown()
{
}

void DeferredRendering::BeginRender()
{
    // TODO: Setup GBuffer and shader values
}

void DeferredRendering::EndRender()
{
}

void DeferredRendering::RenderGeometry()
{
}

void DeferredRendering::RenderShadows()
{
}
