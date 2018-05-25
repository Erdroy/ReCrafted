// ReCrafted (c) 2016-2018 Always Too Late

#include "Object.h"
#include "Scripting/Mono.h"

namespace Internal
{
    void objectDestroy(Object* object)
    {
        Object::Destroy(object);
    }
}

void Object::InitRuntime()
{
    API_FILE("Object.Gen.cs");
    {
        API_COMMENT("Base class for all mono object instances.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API", "Object", PARTIAL);
        {
            API_METHOD(INTERNAL, STATIC, "InternalDestroy", EXTERN);
            {
                API_BIND("ReCrafted.API.Object::InternalDestroy", &Internal::objectDestroy);

                API_PARAM("IntPtr", "nativePtr");
            }
            API_METHOD(INTERNAL, STATIC, "InternalObjectFinalized", EXTERN);
            {
                API_BIND("ReCrafted.API.Object::InternalObjectFinalized", &Object::Finalize);

                API_PARAM("IntPtr", "nativePtr");
            }
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
