// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Action.h"
#include "Common/Event.h"

class ApplicationWindow final
{
    DELETE_OPERATOR_COPY_MOVE(ApplicationWindow)
    DELETE_CTOR_COPY(ApplicationWindow)

public:
    static const uint32_t defaultWidth = 1280;
    static const uint32_t defaultHeight = 720;

private:
    static ApplicationWindow* m_instance;

    void* m_windowHandle = nullptr;
    uint32_t m_width = 0u;
    uint32_t m_height = 0u;
    Action<void> m_onResized = {};
    Event<uint, uint> m_onResizedEvent = {};
    bool m_hasOnResized = false;

public:
    ApplicationWindow();
    ApplicationWindow(ApplicationWindow&& mainLoop) = default;
    ~ApplicationWindow();

public:
    void UpdateSizeNow();

public:
    void SetOnResized(const Action<void>& callback);

    Event<uint, uint>& GetResizeEvent();

public:
    static void WindowResize(void* windowHandle);

    static ApplicationWindow* GetDefault()
    {
        return m_instance;
    }
};
