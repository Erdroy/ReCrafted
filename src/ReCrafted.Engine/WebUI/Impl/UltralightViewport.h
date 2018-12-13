// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ULTRALIGHTVIEWPORT_H
#define ULTRALIGHTVIEWPORT_H

#include "WebUI/WebUIViewport.h"

#include <Ultralight/View.h>

class UltralightViewport : public WebUIViewport, ultralight::LoadListener, ultralight::ViewListener
{
private:
    Renderer::Texture2DHandle m_texture;
    ultralight::RefPtr<ultralight::View> m_view;

    bool m_firstFrame = true;

private:
    void CreateTexture(uint width, uint height);
    void DestroyTexture();

public:
    explicit UltralightViewport(int width, int height, const ultralight::Ref<ultralight::View>& view);
    ~UltralightViewport();

public:
    void Update() override;
    void Render() override;

    void Resize(uint width, uint height) override;

    void Navigate(const char* url) override;
    void Execute(const char* javaScriptSource) override;

    Renderer::Texture2DHandle GetTexture() const override
    {
        return m_texture;
    }

public:
    void OnChangeTitle(ultralight::View* caller, const ultralight::String& title) override;
    void OnChangeURL(ultralight::View* caller, const ultralight::String& url) override;
    void OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip) override;
    void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;
    void OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source, ultralight::MessageLevel level,
        const ultralight::String& message, uint32_t line_number, uint32_t column_number,
        const ultralight::String& source_id) override;
    void OnBeginLoading(ultralight::View* caller) override;
    void OnFinishLoading(ultralight::View* caller) override;
    void OnUpdateHistory(ultralight::View* caller) override;
    void OnDOMReady(ultralight::View* caller) override;
};

#endif // ULTRALIGHTVIEWPORT_H
