// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Core/ApplicationBase.h"
#include "Core/MainLoop.h"

class Application final : public ApplicationBase
{
    DELETE_COPY_MOVE(Application)

private:
    std::shared_ptr<MainLoop> m_mainLoop = nullptr;

public:
    Application();
    ~Application();

private:
    void RegisterSubSystems() const;

public:
    void Run() override;
};
