// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "MainLoop.h"
#include "Time.h"

MainLoop::MainLoop()
{
}

MainLoop::~MainLoop()
{
}

void MainLoop::Run()
{
    m_isRunning = true;

    while(m_isRunning)
    {
        // TODO: Main loop implementation
    }
}

void MainLoop::Quit()
{
    // Quit the main loop next frame
    m_isRunning = false;
}
