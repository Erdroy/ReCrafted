// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/SubSystems/SubSystem.h"

/// <summary>
///     ScriptingManager class. Implements Mono backend for engine scripting.
/// </summary>
class ScriptingManager final : public SubSystem<ScriptingManager>
{
private:
    bool m_attachDebugger = false;

    RefPtr<Assembly> m_apiAssembly;
    RefPtr<Assembly> m_gameAssembly;

private:
    void LoadAssemblies();

protected:
    void Initialize() override;
    void Shutdown() override;
};
