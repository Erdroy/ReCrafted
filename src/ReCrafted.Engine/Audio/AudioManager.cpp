// ReCrafted (c) 2016-2018 Always Too Late

#include "AudioManager.h"
#include "FMOD/FMODEngine.h"
#include "Core/Logger.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(AudioManager)

void AudioManager::OnInit()
{
    Logger::Log("AudioManager initializing...");

    // Initialize audio engine
    m_engine = FMODEngine::GetInstance();
    m_engine->Initialize();

    Logger::Log("AudioManager initialized");
}

void AudioManager::Update()
{
    Profiler::BeginProfile("AudioManager::Update()");
    // Update audio engine
    m_engine->Update();
    Profiler::EndProfile();
}

void AudioManager::OnDispose()
{
    // Shutdown audio engine
    m_engine->Shutdown();
}
