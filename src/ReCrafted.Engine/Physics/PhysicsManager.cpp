// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsManager.h"
#include "Core/Logger.h"

#include <pxphysicsapi.h>

namespace PhysXCallback
{
    class PhysXAllocator : public physx::PxAllocatorCallback
    {
    public:
        void* allocate(size_t size, const char* typeName, const char* fileName, int line) override
        {
            return physx::platformAlignedAlloc(size);
        }

        void deallocate(void* ptr) override
        {
            physx::platformAlignedFree(ptr);
        }
    } PhysXAllocatorCallback;

    class PhysXError : public physx::PxErrorCallback
    {
    public:
        void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override
        {
            Logger::logError("PhysX error code: {0}, message: '{1}' file {2} ({3})", code, message, file, line);
        }
    } PhysXErrorCallback;
}

SINGLETON_IMPL(PhysicsManager)

physx::PxFoundation* m_pxFoundation;
physx::PxPhysics* m_pxPhysics;

void PhysicsManager::onInit()
{
    physx::PxTolerancesScale ToleranceScale;
    ToleranceScale.length = 1;
    ToleranceScale.mass = 1000;
    ToleranceScale.speed = 1000;

    m_pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, PhysXCallback::PhysXAllocatorCallback,
                                        PhysXCallback::PhysXErrorCallback);
    m_pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pxFoundation, ToleranceScale, false);
    //PxInitExtensions(*m_pxPhysics, nullptr);
}

void PhysicsManager::update()
{
}

void PhysicsManager::onDispose()
{
}
