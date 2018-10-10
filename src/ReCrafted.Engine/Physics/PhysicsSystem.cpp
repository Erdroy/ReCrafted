// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsSystem.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"
#include "PhysX/PhysXEngine.h"

RefPtr<IPhysicsEngine> PhysicsSystem::m_engine;

void PhysicsSystem::Initialize()
{
    Logger::Log("PhysicsSystem initializing...");

    m_engine.reset(new PhysXEngine);
    m_engine->Initialize();

    Logger::Log("PhysicsSystem initialized");
}

void PhysicsSystem::Shutdown()
{
    Logger::Log("PhysicsSystem shutdown");
}

void PhysicsSystem::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    m_engine->Update();
    Profiler::EndProfile();
}

void PhysicsSystem::Simulate()
{
    //Profiler::BeginProfile("PhysicsSystem::Simulate");
    m_engine->Simulate();
    //Profiler::EndProfile();
}
