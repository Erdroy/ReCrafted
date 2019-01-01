// ReCrafted (c) 2016-2019 Always Too Late

#include "ApplicationWindow.h"
#include "Platform/Platform.h"

Array<ApplicationWindow*> ApplicationWindow::m_windows;

void ApplicationWindow::Create()
{
    m_Width = 1280;
    m_Height = 720;

    m_windowHandle = Platform::CreateNewWindow(TEXT("ReCrafted"), m_Width, m_Height);

    m_windows.Add(this);
}

void ApplicationWindow::Dispose()
{
    if (m_windowHandle)
        Platform::DestroyWindow(m_windowHandle);

    m_windows.Remove(this);
}

void ApplicationWindow::UpdateSizeNow()
{
    // get size
    Platform::GetWindowSize(m_windowHandle, &m_Width, &m_Height);
}

void ApplicationWindow::SetOnResized(Action<void> callback)
{
    m_onResized = callback;
}

void ApplicationWindow::WindowResize(void* windowHandle)
{
    ApplicationWindow* window = nullptr;

    for (var&& wnd : m_windows)
    {
        if (wnd->m_windowHandle == windowHandle)
        {
            window = wnd;
            break;
        }
    }

    if (!window)
        return;

    var currentWidth = 0u;
    var currentHeight = 0u;

    // get size
    Platform::GetWindowSize(windowHandle, &currentWidth, &currentHeight);

    // check
    if (window->m_Width != currentWidth || window->m_Height != currentHeight)
    {
        window->m_Width = currentWidth;
        window->m_Height = currentHeight;
    }

    // invoke callback if needed
    window->m_onResized.Invoke();
}
