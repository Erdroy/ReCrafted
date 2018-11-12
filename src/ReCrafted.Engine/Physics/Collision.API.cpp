// ReCrafted (c) 2016-2018 Always Too Late

#include "Collision.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    Object* CreateCollision(bool createManagedInstance)
    {
        return Object::CreateInstance<Collision>("ReCrafted.API.Physics", "Collision");
    }
}

void Collision::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Physics.Collision", &Internal::CreateCollision);

    API_FILE("Physics/Collision.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("Collision class");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "Collision", "Object", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
