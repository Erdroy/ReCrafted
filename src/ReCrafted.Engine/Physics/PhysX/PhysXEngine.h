// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXENGINE_H
#define PHYSXENGINE_H

// includes
#include "Physics/IPhysicsEngine.h"
#include "PhysX.h"
#include "PhysXScene.h"

class PhysXEngine : public IPhysicsEngine
{
private:
    PxFoundation* m_foundation = nullptr;
    PxPhysics* m_physics = nullptr;
    PxCooking* m_cooking = nullptr;
    PxDefaultCpuDispatcher* m_cpuDispatcher = nullptr;

    PxTolerancesScale m_tolerance_scale = {};

    std::vector<PhysXScene*> m_scenes = {};

public:
    void Initialize() override;
    void Update() override;
    void Simulate() override;
    void Shutdown() override;

public:
    RefPtr<IPhysicsScene> CreateScene() override;
    void DestroyScene(RefPtr<IPhysicsScene>& scene) override;
};

#endif // PHYSXENGINE_H
