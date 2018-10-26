// ReCrafted (c) 2016-2018 Always Too Late

#include "Actor.h"
#include "Scripting/ScriptingAPI.h"
#include "ActorPool.h"

namespace Internal
{
    Object* CreateActor()
    {
        return ActorPool::AcquireActor();
    }
}

void Actor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Common.Actor", &Internal::CreateActor);

    API_FILE("Common/Actor.Gen.cs");
    {
        API_COMMENT("Actor class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Actor", "Object", PARTIAL);
        {
           
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
