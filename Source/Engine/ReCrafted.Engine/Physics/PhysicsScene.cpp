// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "PhysicsScene.h"
#include "PhysicsManager.h"
#include "MultiThreadStepper.h"
#include "Common/Logger.h"
#include "Core/Time.h"
#include "Rendering/Camera.h"
#include "Rendering/Debug/DebugDraw.h"

PhysicsScene::PhysicsScene()
{
    // Create scene description
    PxSceneDesc sceneDesc(PhysicsManager::GetDefaultTolerances());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f); // No default gravity! We're in space! Gravity is being simulated per RigidBodyActor.
    sceneDesc.bounceThresholdVelocity = 1.0f; // TODO: Tweak this value!
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    sceneDesc.cpuDispatcher = PhysicsManager::GetCPUDispatcher();
    // sceneDesc.simulationEventCallback TODO: Handle simulation events!
    
    // Enable all needed flags
    sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;
    sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;

    // Validate scene description
    ASSERT(sceneDesc.isValid());

    m_scene = PhysicsManager::GetPhysics()->createScene(sceneDesc);
    ASSERT(m_scene);

    // Create controller manager
    m_controllerManager = PxCreateControllerManager(*m_scene);
    ASSERT(m_controllerManager);

    // Setup visualization
    m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_AABBS, 1.0f);

    // Create scratch memory for this scene
    m_scratchMemory = static_cast<uint8_t*>(rc_aligned_alloc(MultiThreadStepper::SCRATCH_BLOCK_SIZE, 16));
    ASSERT(m_scratchMemory);

    PhysicsManager::GetInstance()->AddScene(this);
}

PhysicsScene::~PhysicsScene()
{
    PhysicsManager::GetInstance()->RemoveScene(this);

    if(m_scratchMemory)
    {
        rc_free_aligned(m_scratchMemory);
    }

    PX_RELEASE(m_controllerManager);
    PX_RELEASE(m_scene);
}

void PhysicsScene::Update()
{
    //if (!Profiler::IsPhysicsDebugEnabled())
    /*{
        m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 0.0f);
        return;
    }*/

    auto cp1 = Camera::GetMainCamera()->Position() - Vector3::One * 20.0f;
    auto cp2 = Camera::GetMainCamera()->Position() + Vector3::One * 20.0f;

    // Enable physics visualization
    m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);

    // Set visualization culling box
    m_scene->setVisualizationCullingBox(PxBounds3(*reinterpret_cast<PxVec3*>(&cp1), *reinterpret_cast<PxVec3*>(&cp2)));

    const auto& rb = m_scene->getRenderBuffer();
    for (PxU32 i = 0; i < rb.getNbLines(); i++)
    {
        const auto& line = rb.getLines()[i];

        const auto p1 = *reinterpret_cast<const Vector3*>(&line.pos0);
        const auto p2 = *reinterpret_cast<const Vector3*>(&line.pos1);

        DebugDraw::SetColor(Color::FromHex(line.color0));
        DebugDraw::DrawLine(p1, p2);
    }
}

void PhysicsScene::Simulate()
{
    const auto advanceResult = MultiThreadStepper::GetInstance()->Advance(m_scene, static_cast<PxReal>(Time::FixedDeltaTime()), m_scratchMemory, MultiThreadStepper::SCRATCH_BLOCK_SIZE);

    if (!advanceResult)
        return;

    MultiThreadStepper::GetInstance()->RenderDone(); // TODO: Use Renderer::AddFrameFinish event
    MultiThreadStepper::GetInstance()->Wait(m_scene);
}

bool PhysicsScene::RayCast(const Vector3& position, const Vector3& direction, const float maxDistance, RayCastHit* hit, const uint32_t collisionLayer) const
{
    // Setup query flag
    auto filter = PxQueryFilterData();
    filter.data.word0 = collisionLayer;

    PxRaycastBuffer pxHit;
    const auto isHit = m_scene->raycast(ToPxV3(position), ToPxV3(direction), PxReal(maxDistance), pxHit, PxHitFlag::eDEFAULT, filter);

    if (pxHit.hasAnyHits())
    {
        // We've got some hit! Process the data.
        ASSERT(pxHit.getNbAnyHits() == 1);
        const auto& rayCastHit = pxHit.getAnyHit(0);

        // Copy data to our hit
        hit->Point = FromPxV3(rayCastHit.position);
        hit->Normal = FromPxV3(rayCastHit.normal);
        hit->FaceIndex = rayCastHit.faceIndex;
        hit->Distance = rayCastHit.distance;
    }

    return isHit;
}
