// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef FMODENGINE_H
#define FMODENGINE_H

// includes
#include "ReCrafted.h"
#include "Audio/IAudioEngine.h"

#include <fmod.hpp>

class FMODEngine : public Singleton<FMODEngine>, public IAudioEngine
{
private:
    FMOD::System* m_system = nullptr;

private:
    void OnDispose() override {}

public:
    void Initialize() override;
    void Update() override;
    void Shutdown() override;

public:
    FMOD::System* GetSystem() const
    {
        return m_system;
    }
};

#endif // FMODENGINE_H
