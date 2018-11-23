// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXSCENE_H
#define PHYSXSCENE_H

// includes
#include "ReCrafted.h"
#include "Physics/IPhysicsScene.h"
#include "PhysX.h"
#include "Physics/IPhysicsCharacter.h"

class PhysXScene : public IPhysicsScene
{
    friend class PhysXEngine;

private:
    PxScene* m_scene = nullptr;
    PxControllerManager* m_controllerManager = nullptr;

    void* m_scrathMemory = nullptr;
    PxMaterial* m_defaultMaterial = nullptr;

public:
    PhysXScene(PxCpuDispatcher* cpuDispatcher, PxMaterial* defaultMaterial, const PxTolerancesScale& toleranceScale);
    ~PhysXScene();

protected:
    void Update() override;
    void Simulate() override;
    void Shutdown() override;

public:
    void AttachActor(IPhysicsActor* actor) override;
    void DetachActor(IPhysicsActor* actor) override;

public:
    IPhysicsCharacter* CreateCharacter(float radius, float height, float stepOffset, float slopeLimit, float contactOffset) override;
    void ReleaseCharacter(IPhysicsCharacter* character) override;

public:
    bool RayCast(Vector3 position, Vector3 direction, float maxDistance, RayCastHit* hit) override;
};

#endif // PHYSXSCENE_H
