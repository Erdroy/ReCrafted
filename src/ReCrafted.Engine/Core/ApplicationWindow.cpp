// ReCrafted (c) 2016-2018 Always Too Late

#include "ApplicationWindow.h"
#include "Platform/Platform.h"

void ApplicationWindow::create()
{
    m_windowHandle = Platform::createWindow(TEXT("ReCrafted"));
}

void ApplicationWindow::dispose()
{
    if(m_windowHandle)
        Platform::destroyWindow(m_windowHandle);
}
