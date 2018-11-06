// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsSystem.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"
#include "PhysX/PhysXEngine.h"

RefPtr<IPhysicsEngine> PhysicsSystem::m_engine;

void PhysicsSystem::Initialize()
{
    Logger::Log("PhysicsSystem initializing...");

    m_engine.reset(new PhysXEngine);
    m_engine->Initialize();

    Logger::Log("PhysicsSystem initialized");
}

void PhysicsSystem::Shutdown()
{
    Logger::Log("PhysicsSystem shutdown");
}

void PhysicsSystem::Update()
{
    Profiler::BeginProfile(__FUNCTION__);
    m_engine->Update();

    for (rvar entity : GetEntities())
    {
        rvar transform = entity.GetComponent<TransformComponent>();
        crvar body = entity.GetComponent<PhysicsBodyComponent>();

        cvar position = body.physicsActor->GetPosition();
        transform.position = position;
        transform.rotation = body.physicsActor->GetRotation();

        // TODO: Interpolation/Extrapolation etc.
    }

    Profiler::EndProfile();
}

void PhysicsSystem::Simulate()
{
    //Profiler::BeginProfile("PhysicsSystem::Simulate");
    m_engine->Simulate();
    //Profiler::EndProfile();
}

void PhysicsSystem::OnEntityAdded(const Entity& entity)
{
    rvar transform = entity.GetComponent<TransformComponent>();
    rvar body = entity.GetComponent<PhysicsBodyComponent>();
    rvar shape = entity.GetComponent<PhysicsShapeComponent>();

    ASSERT(body.physicsScene);

    body.physicsActor = m_engine->CreateActor(transform, body);
    shape.physicsShape = m_engine->CreateShape(transform, shape);

    // Attach shape
    body.physicsActor->AttachShape(shape.physicsShape);

    // Attach to scene
    body.physicsScene->AttachActor(body.physicsActor);

    // TODO: Use clustering to attach to proper cluster
}

void PhysicsSystem::OnEntityRemoved(const Entity& entity)
{
    rvar body = entity.GetComponent<PhysicsBodyComponent>();

    if(body.physicsActor)
        m_engine->ReleaseActor(body.physicsActor);
}
