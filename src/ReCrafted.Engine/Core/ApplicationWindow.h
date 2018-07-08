// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

// includes
#include "ReCrafted.h"
#include "IDisposable.h"
#include "Core/Action.h"
#include "Core/Containers/Array.h"

class ApplicationWindow : IDisposable
{
private:
    static Array<ApplicationWindow*> m_windows;

private:
    void* m_windowHandle = nullptr;
    Action<void> m_onResized = {};

private:

public:
    void Create();
    void Dispose() override;

public:
    void UpdateSizeNow();

public:
    void SetOnResized(Action<void> callback);

public:
    static void WindowResize(void* windowHandle);

public:
PROPERTY(uint, Width) = 0u;
PROPERTY(uint, Height) = 0u;
};

#endif // APPLICATIONWINDOW_H
