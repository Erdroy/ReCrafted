// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef MAINLOOP_H
#define MAINLOOP_H

class MainLoop final
{
    DELETE_OPERATOR_COPY_MOVE(MainLoop)
    DELETE_CTOR_COPY(MainLoop)

public:
    MainLoop();
    MainLoop(MainLoop&& mainLoop) = default;
    ~MainLoop();

public:
    void Run();
    void Shutdown();
};

#endif // MAINLOOP_H
