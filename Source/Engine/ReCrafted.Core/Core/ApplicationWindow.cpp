// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ApplicationWindow.h"

List<ApplicationWindow*> ApplicationWindow::m_windows;

ApplicationWindow::ApplicationWindow()
{
    m_width = 720;
    m_height = 1280;

    m_windowHandle = Platform::CreateNewWindow(STRING_CONST("ReCrafted"), m_width, m_height);

    m_windows.Add(this);
}

ApplicationWindow::~ApplicationWindow()
{
    if (m_windowHandle)
        Platform::DestroyWindow(m_windowHandle);

    m_windows.Remove(this);
}

void ApplicationWindow::UpdateSizeNow()
{
    // get size
    Platform::GetWindowSize(m_windowHandle, &m_width, &m_height);
}

void ApplicationWindow::SetOnResized(const Action<void>& callback)
{
    m_onResized = callback;
}

void ApplicationWindow::WindowResize(void* windowHandle)
{
    ApplicationWindow* window = nullptr;

    for (auto&& wnd : m_windows)
    {
        if (wnd->m_windowHandle == windowHandle)
        {
            window = wnd;
            break;
        }
    }

    if (!window)
        return;

    auto currentWidth = 0u;
    auto currentHeight = 0u;

    // get size
    Platform::GetWindowSize(windowHandle, &currentWidth, &currentHeight);

    // check
    if (window->m_width != currentWidth || window->m_height != currentHeight)
    {
        window->m_width = currentWidth;
        window->m_height = currentHeight;
    }

    // invoke callback if needed
    window->m_onResized.Invoke();
}
