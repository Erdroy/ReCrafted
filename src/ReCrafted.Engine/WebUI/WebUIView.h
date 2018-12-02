// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIVIEW_H
#define WEBUIVIEW_H

// includes
#include "ReCrafted.h"
#include "Core/Action.h"
#include "Scripting/Object.h"
#include "WebUIViewport.h"

class WebUIView : public Object
{
    friend class WebUI;

private:
SCRIPTING_API_IMPL()

private:
    int m_width = 0;
    int m_height = 0;
    bool m_fullscreen = false;

    WebUIViewport* m_viewport = nullptr;

private:
    void Init(uint width = 0u, uint height = 0u, bool fullscreen = true);
    void Resize(uint width, uint height);
    void Update();
    void Render();
    void OnDestroy() override;

public:
    void UpdateSurface(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3);

    void Navigate(const char* url);
    void Execute(const char* javaScriptSource);
    void Bind(const char* bindName, Action<void> delegate);

public:
    int Width() const { return m_width; }
    int Height() const { return m_height; }
    int IsFullscreen() const { return m_fullscreen; }

public:
    PROPERTY(bool, Active) = true;
};

#endif // WEBUIVIEW_H
