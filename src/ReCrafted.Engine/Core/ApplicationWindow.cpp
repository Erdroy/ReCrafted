// ReCrafted (c) 2016-2018 Always Too Late

#include "ApplicationWindow.h"
#include "Platform/Platform.h"

Array<ApplicationWindow*> ApplicationWindow::m_windows;

void ApplicationWindow::create()
{
    m_windowHandle = Platform::createWindow(TEXT("ReCrafted"));

    m_windows.add(this);
}

void ApplicationWindow::dispose()
{
    if(m_windowHandle)
        Platform::destroyWindow(m_windowHandle);

    m_windows.remove(this);
}

void ApplicationWindow::setOnResized(Delegate callback)
{
    m_onResized = callback;
}

void ApplicationWindow::windowResize(void* windowHandle)
{
    ApplicationWindow* window = nullptr;

    for(var && wnd : m_windows)
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
    Platform::getWindowSize(windowHandle, &currentWidth, &currentHeight);

    // check
    if(window->m_width != currentWidth || window->m_height != currentHeight)
    {
        window->m_width = currentWidth;
        window->m_height = currentHeight;
    }

    // invoke callback if needed
    window->m_onResized.Invoke();
}
