// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

// includes
#include "IDisposable.h"

class ApplicationWindow : IDisposable
{
private:
    void* m_windowHandle = nullptr;

public:
    void create();
    void dispose() override;
};

#endif // APPLICATIONWINDOW_H
