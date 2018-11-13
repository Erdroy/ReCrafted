// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXScene.h"
#include "Common/Time.h"
#include "PhysXActor.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Camera.h"
#include "Common/Profiler/Profiler.h"

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
    ASSERT(m_scene);

    // Setup visualization
    m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f); 
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

    // Create scratch memory for this scene
    m_scrathMemory = _aligned_malloc(PhysXStepper::SCRATCH_BLOCK_SIZE, 16);
    ASSERT(m_scrathMemory);

    // Initialize substepping
    PhysXStepper::GetInstance()->Initialize(float(Time::FixedDeltaTime()) * 0.25f, 4);
}

PhysXScene::~PhysXScene()
{
    _aligned_free(m_scrathMemory);
    m_scrathMemory = nullptr;
}

void PhysXScene::Update()
{
    if (!Profiler::IsPhysicsDebugEnabled())
    {
        m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 0.0f);
        return;
    }

    var cp1 = Camera::GetMainCamera()->GetPosition() - Vector3::One * 20.0f;
    var cp2 = Camera::GetMainCamera()->GetPosition() + Vector3::One * 20.0f;

    // Enable physics visualization
    m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);

    // Set visualization culling box
    m_scene->setVisualizationCullingBox(PxBounds3(*reinterpret_cast<PxVec3*>(&cp1), *reinterpret_cast<PxVec3*>(&cp2)));
    
    crvar rb = m_scene->getRenderBuffer();
    for (PxU32 i = 0; i < rb.getNbLines(); i++)
    {
        crvar line = rb.getLines()[i];

        cvar p1 = *reinterpret_cast<const Vector3*>(&line.pos0);
        cvar p2 = *reinterpret_cast<const Vector3*>(&line.pos1);

        DebugDraw::SetColor(line.color0);
        DebugDraw::DrawLine(p1, p2);
    }
}

void PhysXScene::Simulate()
{
    cvar advanceResult = PhysXStepper::GetInstance()->Advance(m_scene, static_cast<PxReal>(Time::FixedDeltaTime()), m_scrathMemory, PhysXStepper::SCRATCH_BLOCK_SIZE);

    if(!advanceResult)
        return;

    PhysXStepper::GetInstance()->RenderDone(); // TODO: Use Renderer::SetFrameFinish event
    PhysXStepper::GetInstance()->Wait(m_scene);
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
