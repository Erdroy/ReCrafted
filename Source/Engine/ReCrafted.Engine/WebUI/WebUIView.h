// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Action.h"
#include "Common/Event.h"
#include "Rendering/Texture.h"

#include <Ultralight/Ultralight.h>

/// <summary>
///     WebUIView class. Provides WebUI view rendering and interaction.
/// </summary>
API_CLASS(public, sealed, partial)
class WebUIView final : public Object, public ultralight::LoadListener, ultralight::ViewListener
{
    API_CLASS_BODY()

private:
    String m_name;

    int m_width = 0;
    int m_height = 0;

    bool m_active = true;
    bool m_resize = false;

    Renderer::Texture2DHandle m_texture;
    ultralight::RefPtr<ultralight::View> m_view;

    uint32_t m_skipFrames = 0u;

    Event<> m_onBeginLoading;
    Event<> m_onFinishLoading;
    Event<> m_onDOMReady;

private:
    void CreateTexture(uint width, uint height);
    void DestroyTexture();

public:
    ~WebUIView();

public:
    void Initialize(int width, int height, const ultralight::RefPtr<ultralight::View>& view);

    void Update();
    void Render();

    void RenderFullscreen() const;

public:
    /// <summary>
    ///     Resizes the view port.
    /// </summary>
    API_FUNCTION()
    void Resize(uint width, uint height);

    /// <summary>
    ///     Navigates this view to given URL.
    /// </summary>
    /// <param name="url">The URL to navigate to.</param>
    API_FUNCTION()
    void Navigate(const char* url) const;

    /// <summary>
    ///     Executes given JS source code in the view's JS context.
    /// </summary>
    /// <param name="javaScriptSource">The JS source code.</param>
    API_FUNCTION()
    void Execute(const char* javaScriptSource) const;

    /// <summary>
    ///     Applies current JS context.
    /// </summary>
    void ApplyJSContext() const;

public:
    /// <summary>
    ///     Gets or sets the view's active state. When false, this view will not be processed and rendered.
    /// </summary>
    API_PROPERTY(noprefix)
    void SetActive(bool isActive);

    /// <summary>
    ///     Gets or sets the view's active state. When false, this view will not be processed and rendered.
    /// </summary>
    API_PROPERTY(noprefix)
    bool GetActive() const
    {
        return m_active;
    }

    /// <summary>
    ///     Gets or sets the view's name.
    /// </summary>
    API_PROPERTY(noprefix)
    void SetName(const String& name)
    {
        m_name = name;
    }

    /// <summary>
    ///     Gets or sets the view's name.
    /// </summary>
    API_PROPERTY(noprefix)
    const String& GetName() const
    {
        return m_name;
    }

public:
    void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;
    void OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source, ultralight::MessageLevel level,
        const ultralight::String& message, uint32_t line_number, uint32_t column_number,
        const ultralight::String& source_id) override;

    void OnBeginLoading(ultralight::View* caller) override;
    void OnFinishLoading(ultralight::View* caller) override;
    void OnDOMReady(ultralight::View* caller) override;

public:
    Renderer::Texture2DHandle GetTexture() const
    {
        return m_texture;
    }

    Event<>& BeginLoading() { return m_onBeginLoading; }
    Event<>& FinishLoading() { return m_onFinishLoading; }
    Event<>& DOMReady() { return m_onDOMReady; }

    API_FUNCTION(private)
    void AddOnBeginLoading(const Action<void>& action)
    {
        BeginLoading().AddListener(action);
    }

    API_FUNCTION(private)
    void RemoveOnBeginLoading(const Action<void>& action)
    {
        BeginLoading().RemoveListener(action);
    }

    API_FUNCTION(private)
    void AddOnFinishLoading(const Action<void>& action)
    {
        FinishLoading().AddListener(action);
    }

    API_FUNCTION(private)
    void RemoveOnFinishLoading(const Action<void>& action)
    {
        FinishLoading().RemoveListener(action);
    }

    API_FUNCTION(private)
    void AddOnDOMReady(const Action<void>& action)
    {
        DOMReady().AddListener(action);
    }

    API_FUNCTION(private)
    void RemoveOnDOMReady(const Action<void>& action)
    {
        DOMReady().RemoveListener(action);
    }
};
