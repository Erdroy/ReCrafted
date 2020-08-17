// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ApplicationWindow.h"

ApplicationWindow* ApplicationWindow::m_instance;

ApplicationWindow::ApplicationWindow()
{
    _ASSERT_(m_instance == nullptr, "Only one application window can be created!");

    m_instance = this;

    m_width = defaultWidth;
    m_height = defaultHeight;
    m_windowHandle = Platform::CreateNewWindow(STRING_CONST("ReCrafted"), m_width, m_height);
}

ApplicationWindow::~ApplicationWindow()
{
    if (m_windowHandle)
        Platform::DestroyWindow(m_windowHandle);
    m_instance = nullptr;
}

void ApplicationWindow::UpdateSizeNow()
{
    // get size
    Platform::GetWindowSize(m_windowHandle, &m_width, &m_height);
}

void ApplicationWindow::SetOnResized(const Action<void>& callback)
{
    m_onResized = callback;
    m_hasOnResized = true;
}

Event<uint, uint>& ApplicationWindow::GetResizeEvent()
{
    return m_onResizedEvent;
}

void ApplicationWindow::WindowResize(void* windowHandle)
{
    // NOTE: m_windowHandle will be null when the window is being created while this function will raise 
    // look: ApplicationWindow::ApplicationWindow(): m_windowHandle = Platform::CreateNewWindow...
    if (m_instance == nullptr || m_instance->m_windowHandle == nullptr)
        return;

    ASSERT(m_instance->m_windowHandle == windowHandle);

    auto currentWidth = 0u;
    auto currentHeight = 0u;

    // get size
    Platform::GetWindowSize(windowHandle, &currentWidth, &currentHeight);

    // check
    if (m_instance->m_width != currentWidth || m_instance->m_height != currentHeight)
    {
        m_instance->m_width = currentWidth;
        m_instance->m_height = currentHeight;
    }

    if(m_instance->m_hasOnResized)
    {
        // invoke callback if needed
        m_instance->m_onResized.Invoke();
        m_instance->m_onResizedEvent.Invoke(currentWidth, currentHeight);
    }
}
