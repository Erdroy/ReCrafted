// ReCrafted (c) 2016-2019 Always Too Late

#include "FMODClip.h"
#include "FMODEngine.h"

void FMODClip::Create()
{
    cvar system = FMODEngine::GetInstance()->GetSystem();
    system->createSound(nullptr, FMOD_OPENMEMORY, nullptr, &m_sound);
    
}

void FMODClip::Unload()
{
    if (m_sound)
    {
        m_sound->release();
        m_sound = nullptr;
    }
}
