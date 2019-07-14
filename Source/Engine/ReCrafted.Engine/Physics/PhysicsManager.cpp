// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "PhysicsManager.h"
#include "Common/Logger.h"

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
    m_pvd = PxCreatePvd(*m_foundation);
    m_pvdTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 5000);

    if (m_pvd->connect(*m_pvdTransport, PxPvdInstrumentationFlag::ePROFILE))
        Logger::Log("Connected to NVIDIA PhysX Visual Debugger");
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
    m_defaultMaterial = m_physics->createMaterial(0.7f, 0.7f, 0.25f);

    // Create default shape cooker
    m_shapeCooker = new ShapeCooker();
    m_shapeCooker->Initialize(m_foundation, m_tolerancesScale);
}

void PhysicsManager::Shutdown()
{
    m_cpuDispatcher->release();
    m_cpuDispatcher = nullptr;

    m_shapeCooker->Shutdown();
    delete m_shapeCooker;
    m_shapeCooker = nullptr;

    m_defaultMaterial->release();
    m_defaultMaterial = nullptr;

    PxCloseExtensions();

    m_physics->release();
    m_physics = nullptr;

#ifdef DEBUG
    m_pvdTransport->release();
    m_pvdTransport = nullptr;

    m_pvd->release();
    m_pvd = nullptr;
#endif

    m_foundation->release();
    m_foundation = nullptr;
}

PxPhysics* PhysicsManager::GetPhysics()
{
    return GetInstance()->m_physics;
}

bool PhysicsManager::RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit& hit, uint32_t collisionLayer)
{
    return false;
}
