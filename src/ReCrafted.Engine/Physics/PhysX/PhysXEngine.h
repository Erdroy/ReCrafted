// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXENGINE_H
#define PHYSXENGINE_H

// includes
#include "ReCrafted.h"
#include "Physics/IPhysicsEngine.h"

#include "PhysX.h"
#include "PhysXScene.h"
#include "PhysXActor.h"
#include "PhysXShape.h"
#include "PhysXShapeCooker.h"

class PhysXEngine : public IPhysicsEngine
{
private:
    PxFoundation* m_foundation = nullptr;
    PxPvd* m_pvd = nullptr;
    PxPhysics* m_physics = nullptr;
    PxDefaultCpuDispatcher* m_cpuDispatcher = nullptr;

    PxMaterial* m_defaultMaterial = nullptr;

    PhysXShapeCooker* m_shapeCooker = nullptr;

    std::vector<PhysXScene*> m_scenes = {};

public:
    void Initialize() override;
    void Update() override;
    void Simulate() override;
    void Shutdown() override;

public:
    IPhysicsActor* CreateActor(const TransformComponent& transform, PhysicsBodyComponent& body) override;
    void ReleaseActor(IPhysicsActor* actor) override;

    IPhysicsShape* CreateShape(const TransformComponent& transform, const PhysicsShapeComponent& shape) override;
    void ReleaseShape(IPhysicsShape* shape) override;

    RefPtr<IPhysicsScene> CreateScene() override;
    void DestroyScene(RefPtr<IPhysicsScene>& scene) override;
};

#endif // PHYSXENGINE_H
