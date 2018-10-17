// ReCrafted (c) 2016-2018 Always Too Late

#include "Object.h"
#include "Core/Action.h"
#include "Scripting/Bindings.h"
#include "Scripting/Mono.h"

namespace Internal
{
    API_DEFINE_OBJECT_CREATOR("ReCrafted.API", "Object", Object)

    void objectDestroy(Object* object)
    {
        Object::Destroy(object);
    }

    MonoObject* CreateNew(MonoType* managedType)
    {
        cvar objectFullName = mono_type_full_name(managedType);

        // Get object creator
        cvar objectCreator = Bindings::GetObjectCreator(objectFullName);

        _ASSERT_(objectCreator, "Object creator for this type is not implemented!");

        // Create object
        cvar nativeObject = objectCreator->Invoke();

        // Return managed pointer
        return nativeObject->GetManagedPtr();
    }
}

void Object::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Object", &Internal::CreateObject);

    API_FILE("Object.Gen.cs");
    {
        API_COMMENT("Base class for all mono object instances.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API", "Object", PARTIAL);
        {
            API_METHOD(INTERNAL, STATIC, "InternalNew", EXTERN);
            {
                API_BIND("ReCrafted.API.Object::InternalNew", &Internal::CreateNew);
                
                API_PARAM("IntPtr", "managedType");
                
                API_RETURN("Object");
            }

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
