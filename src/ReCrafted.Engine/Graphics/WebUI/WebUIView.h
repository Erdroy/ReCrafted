// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIVIEW_H
#define WEBUIVIEW_H

// includes
#include "ReCrafted.h"
#include "Scripting/Object.h"

class CEFView;

class WebUIView : public Object
{
    friend class WebUI;

private:
SCRIPTING_API_IMPL()

private:
    CEFView* m_viewBase = nullptr;

private:
    void Init();
    void Update();
    void Resize(uint width, uint height);
    void Render();
    void OnDestroy() override;

public:
    CEFView* GetView() const;

public:
    void Navigate(Text& url);
    void Execute(const char* javaScriptSource);
    void Bind(const char* bindName, Delegate<void> delegate);
};

#endif // WEBUIVIEW_H
