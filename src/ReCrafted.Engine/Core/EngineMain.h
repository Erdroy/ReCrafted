// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINEMAIN_H
#define ENGINEMAIN_H

// includes
#include "ReCrafted.h"
#include "ApplicationBase.h"
#include "ApplicationWindow.h"

/**
 * \brief EngineMain class.
 */
class EngineMain : public ApplicationBase
{
private:
    static EngineMain* m_instance;

private:
    EngineComponentManager* m_componentManager = nullptr;

    Ref<UpdateLoop> m_updateLoop = {};
    Ref<ApplicationWindow> m_mainWindow = {};

    double m_lastUpdateTime = 0.0;

public:
    EngineMain() { m_instance = this; }

    virtual ~EngineMain()
    {
    }

private:
    void RegisterComponents() const;
    void CreateMainWindow();

private:
    void OnSimulate();
    void OnUpdate();
    void OnRender();

    void OnWindowResized();

public:
    void Initialize() override;
    void Run() override;
    void Shutdown() override;

public:
    void Quit();

    FORCEINLINE UpdateLoop* GetUpdateLoop() const
    {
        return m_updateLoop.get();
    }

public:
    static EngineMain* GetInstance()
    {
        return m_instance;
    }
};

#endif // ENGINEMAIN_H
