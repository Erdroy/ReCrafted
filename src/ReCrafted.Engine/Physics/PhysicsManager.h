// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"

class PhysicsManager : public EngineComponent
{
private:
    void onInit() override;
    void update();
    void onShutdown() override;
};

#endif // PHYSICSMANAGER_H
