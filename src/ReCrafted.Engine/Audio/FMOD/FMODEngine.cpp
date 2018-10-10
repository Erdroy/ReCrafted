// ReCrafted (c) 2016-2018 Always Too Late

#include "FMODEngine.h"
#include "Audio/AudioSystem.h"
#include "Core/Logger.h"

SINGLETON_IMPL(FMODEngine)

void FMODEngine::Initialize()
{
    cvar versionMajor = (FMOD_VERSION & 0xFFFF0000) >> 16;
    cvar versionMinor = (FMOD_VERSION & 0x0000FF00) >> 8;
    cvar versionDev = (FMOD_VERSION & 0x000000FF);

    Logger::Log(Text::Format(TEXT_CONST("Initializing FMOD {0}.{1}.{2}"), versionMajor, versionMinor, versionDev).StdStr().c_str());

    // Create system
    ASSERT(FMOD::System_Create(&m_system) == FMOD_OK);

    // Initialize system
    ASSERT(m_system->init(AudioSystem::MaxChannels, FMOD_INIT_NORMAL, nullptr) == FMOD_OK);
}

void FMODEngine::Update()
{
    // Update fmod system
    m_system->update();
}

void FMODEngine::Shutdown()
{
    m_system->release();
    Dispose();
}
