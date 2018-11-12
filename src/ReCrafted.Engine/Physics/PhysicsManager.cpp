// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsManager.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"
#include "PhysX/PhysXEngine.h"

SINGLETON_IMPL(PhysicsManager)

void PhysicsManager::OnInit()
{
    Logger::Log("Physics initializing...");

    m_engine.reset(new PhysXEngine);
    m_engine->Initialize();

    // Create default physics scene
    m_defaultScene = m_engine->CreateScene();

    Logger::Log("Physics initialized");
}

void PhysicsManager::OnDispose()
{
    m_engine->DestroyScene(m_defaultScene);

    Logger::Log("Physics shutdown");
}

void PhysicsManager::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    m_engine->Update();
    Profiler::EndProfile();
}

void PhysicsManager::Simulate()
{
    m_engine->Simulate();
}
