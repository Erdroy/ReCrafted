// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"

class ScriptingEngine : public EngineComponent
{
private:
    Ptr<Domain> m_domain = nullptr;

public:
    void onInit() override;
    void onShutdown() override;
};

#endif // SCRIPTINGENGINE_H
