// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysicsManager.h"
#include "Scripting/ScriptingAPI.h"

void PhysicsManager::InitRuntime()
{
    API_FILE("Physics/PhysicsManager.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("PhysicsManager class");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Physics", "PhysicsManager", PARTIAL);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
