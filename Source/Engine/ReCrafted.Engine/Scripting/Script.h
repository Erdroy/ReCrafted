// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/Actors/ActorBase.h"
#include "Scripting/Object.h"
#include "Scripting/Method.h"

API_USING("ReCrafted.API.Common")
API_USING("ReCrafted.API.Core")
API_USING("ReCrafted.API.Core.Actors")

API_CLASS(public, partial, generic, customNamespace="ReCrafted.API")
class Script : public Object
{
    friend class ActorBase;
    API_CLASS_BODY()


private:
    Method m_awake = Method{ nullptr };
    Method m_start = Method{ nullptr };
    Method m_update = Method{ nullptr };
    Method m_lateUpdate = Method{ nullptr };
    Method m_fixedUpdate = Method{ nullptr };

    Method m_onDestroy = Method{ nullptr };

    Method m_onEnable = Method{ nullptr };
    Method m_onDisable = Method{ nullptr };

    bool m_enabled = false;
    ActorBase* m_actor = nullptr;

private:
    void Initialize();

    void Awake();
    void Start();
    void Update();
    void LateUpdate();
    void FixedUpdate();
    void OnEnable();
    void OnDisable();
    void OnDestroy();

public:
    /// <summary>
    ///     Gets or sets the script's enabled state,
    ///     When script is not enabled, it will not get Update/LateUpdate etc. calls.
    ///     When script is disabled or enabled, this will call it's OnEnable/OnDisable functions.
    /// </summary>
    API_PROPERTY()
    bool Enabled() const;

    /// <summary>
    ///     Gets or sets the script's enabled state,
    ///     When script is not enabled, it will not get Update/LateUpdate etc. calls.
    ///     When script is disabled or enabled, this will call it's OnEnable/OnDisable functions.
    /// </summary>
    API_PROPERTY()
    void Enabled(bool enabled);

    /// <summary>
    ///     Gets the actor that contains this script.
    /// </summary>
    API_FUNCTION(noproxy)
    ActorBase* Actor() const;

    /// <summary>
    ///     Sets the actor that contains this script.
    /// </summary>
    void Actor(ActorBase* actor);
};