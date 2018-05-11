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
    void init();
    void update();
    void resize(uint width, uint height);
    void render();
    void onDestroy() override;

public:
    CEFView* getView() const;

public:
    void navigate(Text& url);
    void execute(const char* javaScriptSource);
    void bind(const char* bindName, Delegate<void> delegate);
};

#endif // WEBUIVIEW_H
