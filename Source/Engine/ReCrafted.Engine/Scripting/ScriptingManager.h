// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/SubSystems/SubSystem.h"

/// <summary>
///     ScriptingManager class. Implements Mono backend for engine scripting.
/// </summary>
class ScriptingManager final : public SubSystem<ScriptingManager>
{
protected:
    void Initialize() override;
    void Shutdown() override;

public:
};
