// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXSCENE_H
#define PHYSXSCENE_H

// includes
#include "Physics/IPhysicsScene.h"

#include <PxPhysicsAPI.h>
using namespace physx;

class PhysXScene : public IPhysicsScene
{
    friend class PhysXEngine;

private:
    PxPhysics* m_physics = nullptr;
    PxScene* m_scene = nullptr;

public:
    PhysXScene(PxPhysics* physics, PxCpuDispatcher* cpuDispatcher, const PxTolerancesScale& toleranceScale);

protected:
    void Update() override;
    void Simulate() override;
    void Shutdown() override;
};

#endif // PHYSXSCENE_H
