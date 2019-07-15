// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "PhysicsManager.h"
#include "Common/Logger.h"
#include "Core/Time.h"
#include "Scripting/Object.h"
#include "MultiThreadStepper.h"

#include <algorithm>
#include "PhysicsMaterial.h"
#include "Common/Platform/Environment.h"

class PhysXAllocator final : public PxAllocatorCallback
{
public:
    void* allocate(const size_t size, const char* typeName, const char* filename, const int line) override
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

class PhysXLogger final : public PxErrorCallback
{
public:
    void reportError(const PxErrorCode::Enum code, const char* message, const char* file, const int line) override
    {
        Logger::LogError("PhysX Error message: {0} - {1} (at line {2})", file, line, message);
    }
};

void PhysicsManager::AddScene(PhysicsScene* scene)
{
    ASSERT(scene);
    MAIN_THREAD_ONLY();

    // Add scene to list
    m_scenes.emplace_back(scene);
}

void PhysicsManager::RemoveScene(PhysicsScene* scene)
{
    ASSERT(scene);
    MAIN_THREAD_ONLY();

    // Remove scene from list
    const auto it = std::find(m_scenes.begin(), m_scenes.end(), scene);
    ASSERT(it != m_scenes.end());
    m_scenes.erase(it);
}

void PhysicsManager::Initialize()
{
    static PhysXAllocator allocator;
    static PhysXLogger logger;

    Logger::Log(String::Format(STRING_CONST("Initializing PhysX {0}.{1}.{2}"), PX_PHYSICS_VERSION_MAJOR, PX_PHYSICS_VERSION_MINOR, PX_PHYSICS_VERSION_BUGFIX).StdStr().c_str());

    // Initialize tolerance scale TODO: Tweak these values
    m_tolerancesScale = {};
    m_tolerancesScale.length = 1.0f;
    m_tolerancesScale.speed = 100.0f; // 100 meters per second
    ASSERT(m_tolerancesScale.isValid());

    // Create px foundation
    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, logger);
    _ASSERT_(m_foundation, "Failed to create PhysX foundation!");

#ifdef DEBUG
    if(Environment::GetCommandLineArguments().Contains(STRING("-pvdConnect")))
    {
        Logger::Log("Connecting to NVIDIA PhysX Visual Debugger...");

        m_pvd = PxCreatePvd(*m_foundation);
        m_pvdTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 5000);

        if (m_pvd->connect(*m_pvdTransport, PxPvdInstrumentationFlag::ePROFILE))
            Logger::Log("Connected to NVIDIA PhysX Visual Debugger.");
        else
            Logger::Log("Could not connect to NVIDIA PhysX Visual Debugger!");
    }
#endif

    // Create px physics
    m_physics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_foundation, m_tolerancesScale, false, m_pvd);
    _ASSERT_(m_physics, "Failed to create PhysX physics!");

#ifdef DEBUG
    // Initialize px extensions
    PxInitExtensions(*m_physics, m_pvd);
#endif

    // Initialize dispatchers
    m_cpuDispatcher = PxDefaultCpuDispatcherCreate(std::min(4u, std::thread::hardware_concurrency() - 1)); // Max. 4 threads for physics TODO: Game settings
    
    // Create default material
    PhysicsMaterial::m_defaultMaterial = Object::New<PhysicsMaterial>();

    // Create default shape cooker
    m_defaultShapeCooker = new ShapeCooker(m_foundation, m_tolerancesScale);

    // Initialize stepper
    MultiThreadStepper::GetInstance()->Initialize(float(Time::FixedDeltaTime()));
}

void PhysicsManager::Shutdown()
{
    Object::DestroyNow(PhysicsMaterial::m_defaultMaterial);

    // Shutdown the stepper
    MultiThreadStepper::GetInstance()->Dispose();

    PX_RELEASE(m_cpuDispatcher);

    delete m_defaultShapeCooker;

    PxCloseExtensions();

    PX_RELEASE(m_physics);

#ifdef DEBUG
    PX_RELEASE(m_pvd);
    PX_RELEASE(m_pvdTransport);
#endif

    PX_RELEASE(m_foundation);
}

void PhysicsManager::OnUpdate()
{
    for (auto&& scene : m_scenes)
    {
        if (scene->Enabled())
        {
            scene->Update();
        }
    }
}

void PhysicsManager::OnFixedUpdate()
{
    for(auto&& scene : m_scenes)
    {
        if (scene->Enabled())
        {
            scene->Simulate();
        }
    }
}

PhysicsScene* PhysicsManager::CreateScene()
{
    return Object::New<PhysicsScene>();
}

PhysicsScene* PhysicsManager::GetSceneAt(Vector3 worldPosition)
{
    auto& lock = GetInstance()->m_scenesLock;
    ScopeLock(lock);

    if (!GetInstance()->m_scenes.empty())
        return GetInstance()->m_scenes[0];

    // TODO: Physics clustering system (PhysicsClusterManager)
    // TODO: Check cluster bounds etc.

    return nullptr;
}

bool PhysicsManager::RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit& hit, uint32_t collisionLayer)
{
    return false;
}
