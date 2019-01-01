// ReCrafted (c) 2016-2019 Always Too Late

#include "Collision.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    MonoObject* CreateBoxCollision(Vector3* extents)
    {
        return Collision::CreateBoxCollision(*extents)->GetManagedPtr();
    }

    MonoObject* CreateSphereCollision(const float radius)
    {
        return Collision::CreateSphereCollision(radius)->GetManagedPtr();
    }

    MonoObject* CreateCapsuleCollision(const float radius, const float halfHeight)
    {
        return Collision::CreateCapsuleCollision(radius, halfHeight)->GetManagedPtr();
    }

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
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Collision class");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "Collision", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(PUBLIC, STATIC, "CreateBoxCollision", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.Collision::CreateBoxCollision", &Internal::CreateBoxCollision);
                API_PARAM("ref Vector3", "extents");
                API_RETURN("Collision");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "CreateSphereCollision", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.Collision::CreateSphereCollision", &Internal::CreateSphereCollision);
                API_PARAM("float", "radius");
                API_RETURN("Collision");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "CreateCapsuleCollision", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.Collision::CreateCapsuleCollision", &Internal::CreateCapsuleCollision);
                API_PARAM("float", "radius");
                API_PARAM("float", "halfHeight");
                API_RETURN("Collision");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
