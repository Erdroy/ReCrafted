// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Application.h"

Application::Application()
{
    m_mainLoop.reset(new MainLoop());
}

Application::~Application()
{
    m_mainLoop.reset();
}

void Application::Run()
{
    m_mainLoop->Run();
}

void Application::Shutdown()
{
    m_mainLoop->Shutdown();
}