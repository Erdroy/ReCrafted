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
    EngineComponentManager* m_componentManager = nullptr;

    Ptr<UpdateLoop> m_updateLoop = {};
    Ptr<ApplicationWindow> m_mainWindow = {};

    double m_lastUpdateTime = 0.0;

public:
    virtual ~EngineMain() {}

private:
    void registerComponents() const;
    void createMainWindow();

private:
    void onSimulate();
    void onUpdate();
    void onRender();

public:
    void initialize() override;
    void run() override;
    void shutdown() override;

public:
    void quit();
};

#endif // ENGINEMAIN_H
