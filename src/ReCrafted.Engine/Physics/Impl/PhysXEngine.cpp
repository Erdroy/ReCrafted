// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXEngine.h"
#include "Core/Logger.h"

#include "PhysXScene.h"

#include <algorithm>

class PhysXAllocator : public PxAllocatorCallback
{
public:

    void* allocate(size_t size, const char* typeName, const char* filename, int line) override
    {
        const auto ptr = malloc(size);
        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
        return ptr;
    }

    void deallocate(void* ptr) override
    {
        free(ptr);
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
    m_tolerance_scale.length = 100.0f;
    m_tolerance_scale.mass = 1000.0f;
    m_tolerance_scale.speed = 500.0f; // 500 meters per second

    // Craete px foundation
    m_foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, allocator, logger);
    _ASSERT_(m_foundation, "Failed to create PhysX foundation!");

    // Create px physics
    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_tolerance_scale, false, nullptr);
    _ASSERT_(m_physics, "Failed to create PhysX physics!");

    // Initialize px extensions
    PxInitExtensions(*m_physics, nullptr);

    // Initialize px cooking, this will be needed for ship colliders cooking etc.
    PxCookingParams cookingParams(m_tolerance_scale);
    cookingParams.meshWeldTolerance = 1.0f / 16.0f; // 1/16 mm tolerance
    cookingParams.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
    cookingParams.targetPlatform = PxPlatform::ePC;
    cookingParams.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;
    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, cookingParams);

    // Initialize dispatchers
    m_cpuDispatcher = PxDefaultCpuDispatcherCreate(std::min(4u, std::thread::hardware_concurrency())); // Max. 4 threads for physics TODO: Game settings

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

    m_cpuDispatcher->release();
    m_cooking->release();
    m_physics->release();
    m_foundation->release();
}

RefPtr<IPhysicsScene> PhysXEngine::CreateScene()
{
    // Create scene and return
    const auto pxScene = new PhysXScene(m_physics, m_cpuDispatcher, m_tolerance_scale);
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
