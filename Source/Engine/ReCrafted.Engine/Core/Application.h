// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Core/ApplicationBase.h"
#include "Core/MainLoop.h"
#include "Core/ApplicationWindow.h"

/// <summary>
///     Application class. Provides application management functionality.
/// </summary>
API_CLASS(public, static, noinherit)
class Application final : public ApplicationBase
{
    DELETE_COPY_MOVE(Application)
    API_CLASS_BODY()

private:
    static Application* m_instance;

    RefPtr<GameManager> m_gameManager = nullptr;
    RefPtr<MainLoop> m_mainLoop = nullptr;
    RefPtr<ApplicationWindow> m_window = nullptr;

public:
    Application() = default;
    ~Application() = default;

private:
    void CreateGameWindow();
    void OnWindowResized();

    void InitializeSubSystems() const;
    void InitializeGame();
    void ShutdownGame();

    void Update();
    void FixedUpdate();
    void Render();

public:
    void Run() override;
    void Shutdown() override;

public:
    /// <summary>
    ///     Quits the current application.
    /// </summary>
    API_FUNCTION();
    static void Quit();
};
