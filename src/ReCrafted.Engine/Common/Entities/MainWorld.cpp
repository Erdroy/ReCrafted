// ReCrafted (c) 2016-2019 Always Too Late

#include "MainWorld.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(MainWorld)

void MainWorld::OnInit()
{
    m_world = Object::CreateInstance<World>("ReCrafted.API.Common.Entities", "World");

    // Register default systems
    m_audioSystem = &m_world->AddSystem<AudioSystem>();
}

void MainWorld::OnDispose()
{
    m_world->Clear();
    Object::Destroy(m_world);
}

void MainWorld::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    m_world->Update();
    Profiler::EndProfile();
}

void MainWorld::Simulate()
{
}
