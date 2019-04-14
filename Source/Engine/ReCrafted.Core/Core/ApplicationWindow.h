// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Action.h"
#include "Common/List.h"

class ApplicationWindow final
{
    DELETE_OPERATOR_COPY_MOVE(ApplicationWindow)
    DELETE_CTOR_COPY(ApplicationWindow)

private:
    static List<ApplicationWindow*> m_windows;

private:
    void* m_windowHandle = nullptr;
    uint32_t m_width = 0u;
    uint32_t m_height = 0u;
    Action<void> m_onResized = {};

public:
    ApplicationWindow();
    ApplicationWindow(ApplicationWindow&& mainLoop) = default;
    ~ApplicationWindow();

public:
    void UpdateSizeNow();

public:
    void SetOnResized(const Action<void>& callback);

public:
    static void WindowResize(void* windowHandle);
};
