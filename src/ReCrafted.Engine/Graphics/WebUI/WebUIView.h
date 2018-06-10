// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIVIEW_H
#define WEBUIVIEW_H

// includes
#include "ReCrafted.h"
#include "Scripting/Object.h"

class WebUIView : public Object
{
    friend class WebUI;

private:
SCRIPTING_API_IMPL()

private:
    int m_width = 0;
    int m_height = 0;
    int m_x = 0;
    int m_y = 0;

    void* m_overlay = nullptr;

private:
    void Init();
    void Update();
    void Resize(uint width, uint height);
    void Render();
    void OnDestroy() override;

public:
    int Width() const { return m_width; }
    int Height() const { return m_height; }
    int X() const { return m_x; }
    int Y() const { return m_y; }

public:
    void Navigate(Text& url);
    void Execute(const char* javaScriptSource);
    void Bind(const char* bindName, Delegate<void> delegate);
};

#endif // WEBUIVIEW_H
