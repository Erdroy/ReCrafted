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

    void RequireComponent(System* system, const uint16_t componentTypeId, const bool nativeComponentId)
    {
        cvar typeId = nativeComponentId ? componentTypeId : ECS_MAX_NATIVE_COMPONENTS + componentTypeId;
        ASSERT(typeId < ECS_MAX_COMPONENTS);
        
        system->RequireComponent(typeId);
    }

    void ExcludeComponent(System* system, const uint16_t componentTypeId, const bool nativeComponentId)
    {
        cvar typeId = nativeComponentId ? componentTypeId : ECS_MAX_NATIVE_COMPONENTS + componentTypeId;
        ASSERT(typeId < ECS_MAX_COMPONENTS);

        system->ExcludeComponent(typeId);
    }

    Entity* GetEntities(System* system, int* numEntities)
    {
        rvar entities = system->GetEntities();

        *numEntities = static_cast<int>(entities.size());
        return const_cast<Entity*>(entities.data());
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

            API_COMMENT("Updates this System. This is called by world.");
            API_METHOD(PROTECTED, ABSTRACT, "Update", UNSAFE);
            {
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "GetEntities", EXTERN, UNSAFE);
            {
                API_BIND("ReCrafted.API.Common.Entities.System::GetEntities", &Internal::GetEntities);

                API_PARAM("IntPtr", "systemNativePtr");
                API_PARAM("int", "numEntities", OUT);
                API_RETURN("Entity*");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "RequireComponent", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.System::RequireComponent", &Internal::RequireComponent);

                API_PARAM("IntPtr", "systemNativePtr");
                API_PARAM("ushort", "componentTypeId");
                API_PARAM("bool", "nativeComponentId");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "ExcludeComponent", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.System::ExcludeComponent", &Internal::ExcludeComponent);

                API_PARAM("IntPtr", "systemNativePtr");
                API_PARAM("ushort", "componentTypeId");
                API_PARAM("bool", "nativeComponentId");
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
