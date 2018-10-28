// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIRenderer.h"
#include "ReCraftedPrerequisites.h"

WebUIRenderer::WebUIRenderer(ultralight::Renderer* renderer, WebUIGPUDriver* driver)
{
    m_renderer = renderer;
    m_driver = driver;
}

WebUIRenderer::~WebUIRenderer()
{
}

void WebUIRenderer::Render(float delta)
{
    m_renderer->Update();
    m_renderer->Render();

    m_needsViewUpdate = m_driver->HasCommandsPending();

    if (m_needsViewUpdate)
        m_driver->DrawCommandList();
}

void WebUIRenderer::Resize(uint16_t width, uint16_t height)
{
    // TODO: Reszie
}
