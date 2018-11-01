// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXScene.h"
#include "Common/Time.h"
#include "PhysXActor.h"

PhysXScene::PhysXScene(PxPhysics* physics, PxCpuDispatcher* cpuDispatcher, const PxTolerancesScale& toleranceScale)
{
    m_physics = physics;

    // Create scene description
    PxSceneDesc sceneDesc(toleranceScale);
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f); // No default gravity! We're in space!
    sceneDesc.bounceThresholdVelocity = 1.0f; // TODO: Tweak this value!
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    // sceneDesc.simulationEventCallback TODO: Handle simulation events!
    sceneDesc.cpuDispatcher = cpuDispatcher;

    // Enable all needed flags
    sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_PAIRS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS;
    sceneDesc.flags |= PxSceneFlag::eENABLE_CCD; // Required AF - for ragdolls etc.
    sceneDesc.flags |= PxSceneFlag::eADAPTIVE_FORCE;

    // Create scene, finally.
    m_scene = m_physics->createScene(sceneDesc);
}

void PhysXScene::Update()
{
}

void PhysXScene::Simulate()
{
    m_scene->simulate(static_cast<PxReal>(Time::FixedDeltaTime()));
    m_scene->fetchResults(true);
}

void PhysXScene::Shutdown()
{
    m_physics = nullptr;
    m_scene->release();
}

void PhysXScene::AttachActor(IPhysicsActor* actor)
{
    ASSERT(actor);

    cvar physxActor = static_cast<PhysXActor*>(actor);

    m_scene->addActor(*physxActor->actor);
}

void PhysXScene::DetachActor(IPhysicsActor* actor)
{
    ASSERT(actor);

    cvar physxActor = static_cast<PhysXActor*>(actor);

    m_scene->removeActor(*physxActor->actor);
}
