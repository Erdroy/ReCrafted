// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/ApplicationBase.h"
#include "Core/MainLoop.h"

class Application final : public ApplicationBase
{
    DELETE_OPERATOR_COPY_MOVE(Application)
    DELETE_CTOR_COPY(Application)

private:
    std::shared_ptr<MainLoop> m_mainLoop = nullptr;

public:
    Application();
    Application(Application&& application) = default;
    ~Application();

public:
    void Run() override;
    void Shutdown() override;
};

#endif // APPLICATION_H
