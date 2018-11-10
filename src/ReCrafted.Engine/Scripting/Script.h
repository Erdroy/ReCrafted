// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

// includes
#include "ReCrafted.h"
#include "Common/ActorBase.h"
#include "Common/GameObject.h"
#include "Scripting/ScriptingAPI.h"

class Script : public Object
{
    friend class GameObject;
    friend class ActorBase;

private:
    SCRIPTING_API_IMPL()

private:
    RefPtr<Method> m_awake = nullptr;
    RefPtr<Method> m_start = nullptr;
    RefPtr<Method> m_update = nullptr;
    RefPtr<Method> m_lateUpdate = nullptr;
    RefPtr<Method> m_simulate = nullptr;

    RefPtr<Method> m_onDestroy = nullptr;

    RefPtr<Method> m_onEnable = nullptr;
    RefPtr<Method> m_onDisable = nullptr;

private:
    void Awake();
    void Start() const;
    void Update() const;
    void LateUpdate() const;
    void Simulate() const;
    void OnDestroy() override;

public:
    PROPERTY(GameObject*, GameObject) = nullptr;
    PROPERTY(ActorBase*, Actor) = nullptr;
    PROPERTY(bool, Enabled) = true;
};

#endif // SCRIPT_H
