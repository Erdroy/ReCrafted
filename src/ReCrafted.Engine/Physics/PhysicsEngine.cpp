// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsEngine.h"
#include "Core/Logger.h"

SINGLETON_IMPL(PhysicsEngine)

void PhysicsEngine::OnInit()
{
    Logger::Log("PhysicsEngine initializing...");

    Logger::Log("PhysicsEngine initialized");
}

void PhysicsEngine::OnDispose()
{
    Logger::Log("PhysicsEngine shutdown");
}

void PhysicsEngine::Update()
{
}

void PhysicsEngine::Simulate()
{
}
