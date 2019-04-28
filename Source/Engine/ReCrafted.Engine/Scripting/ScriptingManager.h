// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Core/SubSystems/SubSystem.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>

class ScriptingManager final : public SubSystem<ScriptingManager>
{
private:
    MonoDomain* m_rootDomain = nullptr;

protected:
    void Initialize() override;
    void Shutdown() override;

public:
};
