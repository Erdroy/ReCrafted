// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

// includes
#include "ReCrafted.h"
#include "IDisposable.h"
#include "Core/Delegate.h"
#include "Core/Containers/Array.h"

class ApplicationWindow : IDisposable
{
private:
    static Array<ApplicationWindow*> m_windows;

private:
    void* m_windowHandle = nullptr;
    Delegate<void> m_onResized = {};

private:

public:
    void create();
    void dispose() override;

public:
    void updateSizeNow();

public:
    void setOnResized(Delegate<void> callback);

public:
    static void windowResize(void* windowHandle);

public:
PROPERTY(uint, width) = 0u;
PROPERTY(uint, height) = 0u;
};

#endif // APPLICATIONWINDOW_H
