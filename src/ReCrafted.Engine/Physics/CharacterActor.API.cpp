// ReCrafted (c) 2016-2018 Always Too Late

#include "CharacterActor.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    Object* CreateCharacter(bool createManagedInstance)
    {
        return Object::CreateInstance<CharacterActor>("ReCrafted.API.Physics", "CharacterActor");
    }
}

void CharacterActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Physics.CharacterActor", &Internal::CreateCharacter);

    API_FILE("Physics/CharacterActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("CharacterActor actor");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "CharacterActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
