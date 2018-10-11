// ReCrafted (c) 2016-2018 Always Too Late

#include "System.h"
#include "Common/EntityComponentSystem.h"

namespace Internal
{
    MonoObject* CreateSystem(MonoType* type)
    {
        cvar systemClass = mono_class_from_mono_type(type);

        RefPtr<Object> systemObject;
        systemObject.reset(new System);

        return Object::Create(systemObject, Domain::Root->GetMono(), systemClass, true);
    }
}

void System::InitRuntime()
{
    API_FILE("Common/Entities/System.Gen.cs")
    {
        API_COMMENT("ECS System class.");
        API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Common.Entities", "System", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Called when this System is being initialized.");
            API_METHOD(PROTECTED, VIRTUAL, "Initialize");
            {
            }
            API_METHOD_END();

            API_COMMENT("Called when this System is being shutdown.");
            API_METHOD(PROTECTED, VIRTUAL, "Shutdown");
            {
            }
            API_METHOD_END();

            API_COMMENT("Updates this System");
            API_METHOD(PUBLIC, ABSTRACT, "Update");
            {
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.System::Create", &Internal::CreateSystem);

                API_PARAM("IntPtr", "systemType");

                API_RETURN("System");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
