// ReCrafted (c) 2016-2018 Always Too Late

#include "World.h"
#include "Common/EntityComponentSystem.h"
#include "MainWorld.h"

namespace Internal
{
    void AddSystem(World* world, System* system, const uint16_t systemTypeId)
    {
        cvar typeId = ECS_MAX_NATIVE_SYSTEMS + systemTypeId;
        ASSERT(typeId < ECS_MAX_SYSTEMS);
        world->AddSystem(system, typeId);
    }

    void RemoveSystem(World* world, const uint16_t systemTypeId)
    {
        cvar typeId = ECS_MAX_NATIVE_SYSTEMS + systemTypeId;
        ASSERT(typeId < ECS_MAX_SYSTEMS);

        world->RemoveSystem(typeId);
    }

    bool HasSystem(World* world, const uint16_t systemTypeId)
    {
        cvar typeId = ECS_MAX_NATIVE_SYSTEMS + systemTypeId;
        ASSERT(typeId < ECS_MAX_SYSTEMS);

        return world->HasSystem(typeId);
    }

    MonoObject* CreateWorld(MonoString* name)
    {
        cvar world = Object::CreateInstance<World>("ReCrafted.API.Common", "Entity");
        return world->GetManagedPtr();
    }

    MonoObject* GetMainWorld()
    {
        ASSERT(MainWorld::GetWorld());
        cvar mainWorldPtr = MainWorld::GetWorld()->GetManagedPtr();
        return mainWorldPtr;
    }
}

void World::InitRuntime()
{
    API_FILE("Common/Entities/World.Gen.cs")
    {
        API_COMMENT("ECS World class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common.Entities", "World", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Updates this World");
            API_METHOD(PUBLIC, REGULAR, "Update", EXTERN);
            {
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "AddSystem", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.World::AddSystem", &Internal::AddSystem);

                API_PARAM("IntPtr", "worldNativePtr");
                API_PARAM("IntPtr", "systemNativePtr");
                API_PARAM("ushort", "systemId");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "RemoveSystem", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.World::RemoveSystem", &Internal::RemoveSystem);

                API_PARAM("IntPtr", "worldNativePtr");
                API_PARAM("ushort", "systemId");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, STATIC, "HasSystem", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.World::HasSystem", &Internal::HasSystem);

                API_PARAM("IntPtr", "worldNativePtr");
                API_PARAM("ushort", "systemId");

                API_RETURN("bool");
            }
            API_METHOD_END();


            API_COMMENT("Creates new World");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                //API_BIND("ReCrafted.API.Common.Entities.World::Create", &Internal::CreateWorld);

            }
            API_METHOD_END();

            API_COMMENT("Creates new World");
            API_METHOD(PUBLIC, STATIC, "GetMainWorld", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.World::GetMainWorld", &Internal::GetMainWorld);
                API_RETURN("World");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
