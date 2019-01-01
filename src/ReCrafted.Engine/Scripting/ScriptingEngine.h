// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef SCRIPTINGENGINE_H
#define SCRIPTINGENGINE_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"

class ScriptingEngine : public EngineComponent<ScriptingEngine>
{
private:
    RefPtr<Domain> m_domain = nullptr;

public:
    virtual ~ScriptingEngine() = default;

public:
    void OnInit() override;
    void OnDispose() override;

public:
    static void Finalize();
    static void AttachCurrentThread();
    static void DetachCurrentThread();
};

#endif // SCRIPTINGENGINE_H
