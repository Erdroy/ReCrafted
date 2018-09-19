// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsManager.h"
#include "Core/Logger.h"

#include "Impl/PhysXEngine.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(PhysicsManager)

void PhysicsManager::OnInit()
{
    Logger::Log("PhysicsManager initializing...");

    m_engine.reset(new PhysXEngine);
    m_engine->Initialize();

    Logger::Log("PhysicsManager initialized");
}

void PhysicsManager::OnDispose()
{
    Logger::Log("PhysicsManager shutdown");
}

void PhysicsManager::Update()
{
    Profiler::BeginProfile("PhysicsManager::Update()");
    m_engine->Update();
    Profiler::EndProfile();
}

void PhysicsManager::Simulate()
{
    //Profiler::BeginProfile("PhysicsManager::Simulate()");
    m_engine->Simulate();
    //Profiler::EndProfile();
}
