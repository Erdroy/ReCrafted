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
    void init(Entity* entity)
    {
        m_entity = entity;

        m_onUpdate = findMethod("ReCrafted.API.Common.Script::OnUpdate");
        m_onSimulate = findMethod("ReCrafted.API.Common.Script::OnSimulate");
        m_onDestroy = findMethod("ReCrafted.API.Common.Script::OnDestroy");
    }

    FORCEINLINE void update() const
    {
        if (m_onUpdate)
            m_onUpdate->invoke();
    }

    FORCEINLINE void simulate() const
    {
        if (m_onSimulate)
            m_onSimulate->invoke();
    }

    void onDestroy() override
    {
        if (m_onDestroy)
            m_onDestroy->invoke();
    }

public:
    void dispose();
};

#endif // SCRIPT_H
