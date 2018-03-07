// ReCrafted (c) 2016-2018 Always Too Late

#include "Renderer.h"

#include "Common/Display.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "bgfxPrerequisites.h"

SINGLETON_IMPL(Renderer)

#define RESET_FLAGS (BGFX_RESET_NONE)

Renderer* g_rendererInstance;

void Renderer::bgfx_initialize()
{
    Logger::logInfo("Creating renderer with Direct3D11 API");

    // initialize bgfx platform data
    bgfx::PlatformData pd;
    memset(&pd, 0, sizeof(pd));
    pd.nwh = Platform::getCurrentWindow();
    bgfx::setPlatformData(pd);

    // initialize rendering
    bgfx::init(bgfx::RendererType::Direct3D11);
    bgfx::reset(Display::get_Width(), Display::get_Height(), RESET_FLAGS);

    bgfx::setDebug(BGFX_DEBUG_NONE);
}

void Renderer::bgfx_shutdown()
{
}

void Renderer::onInit()
{
    g_rendererInstance = this;
}

void Renderer::onDispose()
{
}

void Renderer::update()
{
}

void Renderer::render()
{
}
