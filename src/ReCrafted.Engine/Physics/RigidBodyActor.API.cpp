// ReCrafted (c) 2016-2018 Always Too Late

#include "RigidBodyActor.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    Object* CreateRigidBodyActor(bool createManagedInstance)
    {
        return RigidBodyActor::Create();
    }
}

const char* Actor<RigidBodyActor>::ManagedName = "RigidBodyActor";
const char* Actor<RigidBodyActor>::ManagedNamespace = "ReCrafted.API.Physics";

void RigidBodyActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Physics.RigidBodyActor", &Internal::CreateRigidBodyActor);

    API_FILE("Physics/RigidBodyActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");
        API_USING("ReCrafted.API.Common.Actors");

        API_COMMENT("RigidBodyActor actor");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "RigidBodyActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
