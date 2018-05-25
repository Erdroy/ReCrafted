// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

// includes
#include "Object.h"
#include "Core/Defines.h"
#include "Method.h"

class Entity;

class Script : public Object
{
private:
SCRIPTING_API_IMPL()

private:
    Entity* m_entity = nullptr;

    Ref<Method> m_onUpdate = nullptr;
    Ref<Method> m_onSimulate = nullptr;
    Ref<Method> m_onDestroy = nullptr;

public:
    void Init(Entity* entity)
    {
        m_entity = entity;

        m_onUpdate = FindMethod("ReCrafted.API.Common.Script::OnUpdate");
        m_onSimulate = FindMethod("ReCrafted.API.Common.Script::OnSimulate");
        m_onDestroy = FindMethod("ReCrafted.API.Common.Script::OnDestroy");
    }

    FORCEINLINE void Update() const
    {
        if (m_onUpdate)
            m_onUpdate->Invoke();
    }

    FORCEINLINE void Simulate() const
    {
        if (m_onSimulate)
            m_onSimulate->Invoke();
    }

    void OnDestroy() override
    {
        if (m_onDestroy)
            m_onDestroy->Invoke();
    }

public:
    void Dispose();
};

#endif // SCRIPT_H
