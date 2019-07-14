// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "PhysicsManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static bool PhysicsManager_RayCast1(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit* hit, uint32_t collisionLayer) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = PhysicsManager::RayCast(position, direction, maxDistance, *hit, collisionLayer);
        return _returnValue;
    }
};

void PhysicsManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(PhysicsManager);
    API_BIND("ReCrafted.API.Physics.PhysicsManager::InternalRayCast", &APIProxy::PhysicsManager_RayCast1);
}

const char* PhysicsManager::Fullname() 
{
    return "ReCrafted.API.Physics.PhysicsManager";
}

const char* PhysicsManager::Name() 
{
    return "PhysicsManager";
}

const char* PhysicsManager::Namespace() 
{
    return "ReCrafted.API.Physics";
}