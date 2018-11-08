// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXEngine.h"
#include "Core/Logger.h"

#include "Common/TransformComponent.h"
#include "Physics/Components/PhysicsBodyComponent.h"
#include "Physics/Components/PhysicsShapeComponent.h"

#include <algorithm>

PxPhysics* GPxPhysX;
PxFoundation* GPxFoundation;
PxTolerancesScale GPxTolerances;

class PhysXAllocator : public PxAllocatorCallback
{
public:

    void* allocate(size_t size, const char* typeName, const char* filename, int line) override
    {
        const auto ptr = rc_malloc(size);
        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
        return ptr;
    }

    void deallocate(void* ptr) override
    {
        rc_free(ptr);
    }
};

class PhysXLogger : public PxErrorCallback
{
public:
    void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override
    {
        const auto log = Text::Format(TEXT_CONST("PhysX Error message: {0} - {1} (at line {2})."), file, line, message);
        Logger::LogError(log.StdStr().c_str());
    }
};

void PhysXEngine::Initialize()
{
    static PhysXAllocator allocator;
    static PhysXLogger logger;

    Logger::Log(Text::Format(TEXT_CONST("Initializing PhysX {0}.{1}.{2}"), PX_PHYSICS_VERSION_MAJOR, PX_PHYSICS_VERSION_MINOR, PX_PHYSICS_VERSION_BUGFIX).StdStr().c_str());

    // Initialize tolerance scale TODO: Tweak these values
    GPxTolerances = {};
    GPxTolerances.length = 1.0f;
    GPxTolerances.mass = 1000.0f;
    GPxTolerances.speed = 10.0f; // 100 meters per second

    // Craete px foundation
    GPxFoundation = m_foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, allocator, logger);
    _ASSERT_(m_foundation, "Failed to create PhysX foundation!");

#ifdef DEBUG
    m_pvd = PxCreatePvd(*m_foundation);
    const auto transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 5000);

    if(transport && m_pvd->connect(*transport, PxPvdInstrumentationFlag::eDEBUG))
        Logger::Log("Connected to nVidia PhysX Visual Debugger");
#endif

    // Create px physics
    GPxPhysX = m_physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_foundation, GPxTolerances, true, m_pvd);
    _ASSERT_(m_physics, "Failed to create PhysX physics!");

#ifndef _DEBUG
    // Initialize px extensions
    PxInitExtensions(*m_physics, m_pvd);
#endif

    // Create default cooker
    m_shapeCooker = new PhysXShapeCooker();
    m_shapeCooker->Initialize({});

    // Initialize dispatchers
    m_cpuDispatcher = PxDefaultCpuDispatcherCreate(std::min(4u, std::thread::hardware_concurrency() - 1)); // Max. 4 threads for physics TODO: Game settings

    m_defaultMaterial = m_physics->createMaterial(0.7f, 0.7f, 0.25f);

    // TODO: Optional CUDA support
}

void PhysXEngine::Update()
{
    for (auto& scene : m_scenes)
    {
        if (scene)
            scene->Update();
    }
}

void PhysXEngine::Simulate()
{
    for (auto& scene : m_scenes)
    {
        if (scene)
            scene->Simulate();
    }
}

void PhysXEngine::Shutdown()
{
    for (auto& scene : m_scenes)
    {
        if (scene)
            scene->Shutdown();
    }

    m_scenes.clear();
    
    m_shapeCooker->Shutdown();

    m_pvd->release();
    m_cpuDispatcher->release();
    m_physics->release();
    m_foundation->release();
}

IPhysicsActor* PhysXEngine::CreateActor(const TransformComponent& transform, PhysicsBodyComponent& body)
{
    rvar position = transform.position;
    rvar rotation = transform.rotation; // TODO: Set pxTransform rotation when we will have our new math

    cvar pxTransform = PxTransform(position.x, position.y, position.z);
    
    PxRigidActor* pxActor;
    if (body.type == PhysicsBodyComponent::Dynamic)
        pxActor = m_physics->createRigidDynamic(pxTransform);
    else
        pxActor = m_physics->createRigidStatic(pxTransform);

    // Return new PhysX Actor Impl.
    return new PhysXActor(pxActor);
}

void PhysXEngine::ReleaseActor(IPhysicsActor* actor)
{
    ASSERT(actor);

    var physxActor = static_cast<PhysXActor*>(actor);
    
    if(physxActor->actor)
        physxActor->actor->release();

    delete actor;
}

IPhysicsShape* PhysXEngine::CreateShape(const TransformComponent& transform, const PhysicsShapeComponent& shape)
{
    rvar extents = shape.extents;

    PxShape* pxShape = nullptr;
    switch (shape.type)
    {
    case PhysicsShapeComponent::Box:
    {
        pxShape = m_physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *m_defaultMaterial);
        break;
    }
    case PhysicsShapeComponent::Sphere:
    {
        pxShape = m_physics->createShape(PxSphereGeometry(shape.radius), *m_defaultMaterial);
        break;
    }
    case PhysicsShapeComponent::TriangleMesh:
    {
        ASSERT(shape.shapePointer);

        cvar pxTriangleMesh = static_cast<PxTriangleMesh*>(shape.shapePointer);

        // Create shape
        pxShape = m_physics->createShape(PxTriangleMeshGeometry(pxTriangleMesh,
            PxMeshScale(), PxMeshGeometryFlag::eDOUBLE_SIDED), *m_defaultMaterial);
        break;
    }
    default:
        break;
    }

    ASSERT(pxShape);

    pxShape->setLocalPose(PxTransform(0.0f, 0.0f, 0.0f));

    // Return new PhysX Shape Impl.
    return new PhysXShape(pxShape);
}

void PhysXEngine::ReleaseShape(IPhysicsShape* shape)
{
    ASSERT(shape);

    var physxShape = static_cast<PhysXShape*>(shape);

    if (physxShape->shape)
        physxShape->shape->release();

    delete shape;
}

RefPtr<IPhysicsScene> PhysXEngine::CreateScene()
{
    // Create scene and return
    const auto pxScene = new PhysXScene(m_physics, m_cpuDispatcher, GPxTolerances);
    RefPtr<IPhysicsScene> physicsScene(pxScene);

    // Add scene
    m_scenes.emplace_back(pxScene);

    return physicsScene;
}

void PhysXEngine::DestroyScene(RefPtr<IPhysicsScene>& scene)
{
    const auto scenePtr = static_cast<PhysXScene*>(scene.get());

    // Find and remove scene
    const auto it = std::find(m_scenes.begin(), m_scenes.end(), scenePtr);
    if (it != m_scenes.end())
    {
        m_scenes.erase(it);
    }

    // Reset pointer.
    scene.reset();
}
