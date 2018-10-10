// ReCrafted (c) 2016-2018 Always Too Late

#include "MainWorld.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(MainWorld)

void MainWorld::OnInit()
{
    m_world.reset(new World());

    // Register default systems
    m_audioSystem = &m_world->AddSystem<AudioSystem>();
    m_physicsSystem = &m_world->AddSystem<PhysicsSystem>();
}

void MainWorld::OnDispose()
{
    m_world->Clear();
    m_world.reset();
}

void MainWorld::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    m_world->Update();
    Profiler::EndProfile();
}

void MainWorld::Simulate()
{
    m_physicsSystem->Simulate();
}
