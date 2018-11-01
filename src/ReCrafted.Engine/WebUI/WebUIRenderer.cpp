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

void WebUIRenderer::Update()
{
    m_renderer->Update();
}

void WebUIRenderer::Render()
{
    m_renderer->Render();

    m_needsViewUpdate = m_driver->HasCommandsPending();

    if (m_needsViewUpdate)
    {
        m_driver->DrawCommandList();
    }
}

void WebUIRenderer::Resize(uint16_t width, uint16_t height)
{
    // TODO: Reszie
}
