// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class MainLoop final
{
    DELETE_OPERATOR_COPY_MOVE(MainLoop)
    DELETE_CTOR_COPY(MainLoop)

private:
    bool m_isRunning = false;

public:
    MainLoop();
    MainLoop(MainLoop&& mainLoop) = default;
    ~MainLoop();

public:
    void Run();
    void Shutdown();
};
