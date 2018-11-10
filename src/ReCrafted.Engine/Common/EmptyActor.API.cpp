// ReCrafted (c) 2016-2018 Always Too Late

#include "EmptyActor.h"

namespace Internal
{
    Object* CreateEmptyActor(bool createManagedInstance)
    {
        return EmptyActor::Create();
    }
}

const char* Actor<EmptyActor>::ManagedName = "EmptyActor";
const char* Actor<EmptyActor>::ManagedNamespace = "ReCrafted.API.Common";

void EmptyActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Common.EmptyActor", &Internal::CreateEmptyActor);

    API_FILE("Common/EmptyActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("EmptyActor class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "EmptyActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
