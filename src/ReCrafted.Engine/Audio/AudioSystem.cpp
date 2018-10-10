// ReCrafted (c) 2016-2018 Always Too Late

#include "AudioSystem.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"
#include "FMOD/FMODEngine.h"

IAudioEngine* AudioSystem::m_engine;

void AudioSystem::Initialize()
{
    Logger::Log("AudioSystem initializing...");

    // Initialize audio engine
    m_engine = FMODEngine::GetInstance();
    m_engine->Initialize();

    Logger::Log("AudioSystem initialized");
}

void AudioSystem::Shutdown()
{
    // Shutdown audio engine
    m_engine->Shutdown();
}

void AudioSystem::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    // Update audio engine
    m_engine->Update();



    Profiler::EndProfile();
}
