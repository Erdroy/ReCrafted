// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"
#include "WebUI/WebUIView.h"
#include "WebUI/Impl/GPUContext.h"

#include <Ultralight/Ultralight.h>

/// <summary>
///     WebUIManager class. Provides WebUIView management, UI engine initialization and shutdown and more.
/// </summary>
API_CLASS(internal, static, partial, noinherit)
class WebUIManager final : public SubSystem<WebUIManager>
{
    API_CLASS_BODY()
    friend class WebUIView;

private:
    std::unique_ptr<ultralight::GPUContext> m_context = nullptr;
    ultralight::RefPtr<ultralight::Renderer> m_renderer = nullptr;

    List<WebUIView*> m_views = {};

protected:
    void Initialize() override;
    void Shutdown() override;

    void OnUpdate() override;

public:
    void Render();
    void RenderViews();

    void ResizeFullscreenViews(uint width, uint height);

public:
    /// <summary>
    /// Creates new <see cref="WebUIView"/> of given width and height.
    /// </summary>
    /// <param name="width">The width.</param>
    /// <param name="height">The height.</param>
    /// <returns>The created WebUIView.</returns>
    API_FUNCTION()
    static WebUIView* CreateView(int width, int height);
};
