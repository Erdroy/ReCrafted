// ReCrafted (c) 2016-2018 Always Too Late

#include "ScriptingSystem.h"
#include "Common/Profiler/Profiler.h"
#include "Script.h"

void ScriptingSystem::Initialize()
{
}

void ScriptingSystem::Shutdown()
{
}

void ScriptingSystem::Update()
{
    Profiler::BeginProfile(__FUNCTION__);

    auto entities = GetEntities();

    for(auto& entity : entities)
    {
        auto& transformComponent = entity.GetComponent<TransformComponent>();
        auto& scriptingComponent = entity.GetComponent<ScriptingComponent>();

        for(auto& script : scriptingComponent.scripts)
        {
            if(!script)
                continue;

            script->Update();
        }
    }

    Profiler::EndProfile();
}

void ScriptingSystem::Simulate()
{
    //Profiler::BeginProfile(__FUNCTION__);
    auto entities = GetEntities();

    for (auto& entity : entities)
    {
        auto& scriptingComponent = entity.GetComponent<ScriptingComponent>();

        for (auto& script : scriptingComponent.scripts)
        {
            script->Simulate();
        }
    }
    //Profiler::EndProfile();
}
