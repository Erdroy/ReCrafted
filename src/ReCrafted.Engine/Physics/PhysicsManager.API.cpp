// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsManager.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    bool RayCast(Vector3* origin, Vector3* direction, const float maxDistance, RayCastHit* hit, const uint32_t collisionLayers)
    {
        return PhysicsManager::RayCast(*origin, *direction, maxDistance, hit, collisionLayers);
    }
}

void PhysicsManager::InitRuntime()
{
    API_FILE("Physics/PhysicsManager.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("PhysicsManager class");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Physics", "PhysicsManager", PARTIAL);
        {
            API_METHOD(INTERNAL, STATIC, "RayCast", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.PhysicsManager::RayCast", &Internal::RayCast);
                API_PARAM("ref Vector3", "origin");
                API_PARAM("ref Vector3", "direction");
                API_PARAM("float", "maxDistance");
                API_PARAM("out RayCastHit", "hit");
                API_PARAM("uint", "collisionLayers");

                API_RETURN("bool");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
