// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorBase.h"

namespace Internal
{
}

void ActorBase::InitRuntime()
{
    //API_REGISTER_OBJECT("ReCrafted.API.Common.ActorBase", &Internal::CreateActor);

    API_FILE("Common/ActorBase.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("ActorBase class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "ActorBase", "Object", PARTIAL, NOCONSTRUCTOR);
        {

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
