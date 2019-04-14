// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Core/ApplicationBase.h"
#include "Core/MainLoop.h"
#include "Core/ApplicationWindow.h"

class Application final : public ApplicationBase
{
    DELETE_COPY_MOVE(Application)

private:
    RefPtr<MainLoop> m_mainLoop = nullptr;
    RefPtr<ApplicationWindow> m_window = nullptr;

public:
    Application();
    ~Application();

private:
    void CreateGameWindow();
    void OnWindowResized();

    void RegisterSubSystems() const;

    void Update();
    void FixedUpdate();
    void Render();

public:
    void Run() override;
};
