// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXEngine.h"
#include "Core/Logger.h"

#include "Common/TransformComponent.h"
#include "Physics/Components/PhysicsBodyComponent.h"
#include "Physics/Components/PhysicsShapeComponent.h"

#include <particles/PxParticleBase.h>
#include <algorithm>
#include "PhysXCharacter.h"

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

    // Register particle module
    PxRegisterParticles(*m_physics);

#ifndef _DEBUG
    // Initialize px extensions
    PxInitExtensions(*m_physics, m_pvd);
#endif

    // Create default cooker
    m_shapeCooker = new PhysXShapeCooker();
    m_shapeCooker->Initialize({});

    // Create controller manager
    // m_controllerManager = PxCreateControllerManager(); // TODO: Per-scene controller manager (?)

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

IPhysicsShapeCooker* PhysXEngine::CreateCooker()
{
    return new PhysXShapeCooker();
}

void PhysXEngine::ReleaseCooker(IPhysicsShapeCooker* cooker)
{
    ASSERT(cooker);
    cooker->Shutdown();
    delete cooker;
}

IPhysicsCharacter* PhysXEngine::CreateCharacter(float radius, float height, float stepOffset, float slopeLimit, float contactOffset)
{
    // Create controller description
    PxCapsuleControllerDesc controllerDesc;
    controllerDesc.radius = radius;
    controllerDesc.height = height;
    controllerDesc.stepOffset = stepOffset;
    controllerDesc.slopeLimit = slopeLimit;
    controllerDesc.contactOffset = contactOffset;
    controllerDesc.material = m_defaultMaterial;

    // Make sure that we've got proper controller description
    ASSERT(controllerDesc.isValid());

    // Create controller
    cvar pxController = m_controllerManager->createController(controllerDesc);

    ASSERT(pxController);

    return new PhysXCharacter(static_cast<PxCapsuleController*>(pxController));
}

void PhysXEngine::ReleaseCharacter(IPhysicsCharacter* character)
{
}

IPhysicsActor* PhysXEngine::CreateActor(const Transform& transform, const bool dynamic)
{
    rvar position = transform.translation;
    rvar rotation = transform.orientation;

    // BUG: Fix actor transform initialization

    cvar pxTransform = PxTransform(position.x, position.y, position.z/*, PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)*/);
    
    ASSERT(pxTransform.isValid());

    PxRigidActor* pxActor;
    if (dynamic)
        pxActor = m_physics->createRigidDynamic(pxTransform);
    else
        pxActor = m_physics->createRigidStatic(pxTransform);

    ASSERT(pxActor);

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

IPhysicsShapeCooker* PhysXEngine::GetDefaultCooker()
{
    ASSERT(m_shapeCooker);
    return m_shapeCooker;
}

IPhysicsShape* PhysXEngine::CreateBoxShape(const Vector3& extents)
{
    return new PhysXShape(m_physics->createShape(PxBoxGeometry(extents.x, extents.y, extents.z), *m_defaultMaterial));
}

IPhysicsShape* PhysXEngine::CreateSphereShape(const float radius)
{
    return new PhysXShape(m_physics->createShape(PxSphereGeometry(radius), *m_defaultMaterial));
}

IPhysicsShape* PhysXEngine::CreateCapsuleShape(const float radius, const float halfHeight)
{
    return new PhysXShape(m_physics->createShape(PxCapsuleGeometry(radius, halfHeight), *m_defaultMaterial));
}

IPhysicsShape* PhysXEngine::CreateTriangleMeshShape(void* shapePtr)
{
    ASSERT(shapePtr);

    cvar pxTriangleMesh = static_cast<PxTriangleMesh*>(shapePtr);

    // Create shape
    return new PhysXShape(m_physics->createShape(PxTriangleMeshGeometry(pxTriangleMesh,
        PxMeshScale(), PxMeshGeometryFlag::eDOUBLE_SIDED), *m_defaultMaterial));
}

IPhysicsShape* PhysXEngine::CreateConvexHullMeshShape(void* shapePtr)
{
    ASSERT(shapePtr);

    cvar pxHullMesh = static_cast<PxConvexMesh*>(shapePtr);

    // Create shape
    return new PhysXShape(m_physics->createShape(PxConvexMeshGeometry(pxHullMesh, PxMeshScale()), *m_defaultMaterial));
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
