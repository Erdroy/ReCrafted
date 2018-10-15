// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"

namespace Internal
{
    void AddComponent(World* world, const EntityId entityId, void* componentData, const uint32_t componentDataSize, const TypeId componentId, const bool nativeComponentId)
    {
        ASSERT(world);
        ASSERT(componentData);
        ASSERT(componentDataSize < ECS_MAX_COMPONENT_SIZE);

        cvar typeId = nativeComponentId ? componentId : ECS_MAX_NATIVE_COMPONENTS + componentId;
        ASSERT(typeId < ECS_MAX_COMPONENTS);

        cvar manager = world->GetEntityManager();
        ASSERT(manager->IsEntityValid(entityId));

        // Allocate component
        // We must be sure that delete is using rc_free (in EntityManager),
        // because we cannot allocate component from type right there
        // and it forces us to use rc_malloc.
        cvar component = static_cast<Component*>(rc_malloc(componentDataSize));

        // Copy data
        memcpy_s(component, componentDataSize, componentData, componentDataSize);

        // Add component
        manager->AddEntityComponent(entityId, component, typeId);
    }

    Component* GetComponent(World* world, const EntityId entityId, const TypeId componentId, const bool nativeComponentId)
    {
        ASSERT(world);

        cvar typeId = nativeComponentId ? componentId : ECS_MAX_NATIVE_COMPONENTS + componentId;
        ASSERT(typeId < ECS_MAX_COMPONENTS);

        cvar manager = world->GetEntityManager();
        ASSERT(manager->IsEntityValid(entityId));
        
        return &manager->GetEntityComponent(entityId, typeId);
    }

    void RemoveComponent(World* world, const EntityId entityId, const TypeId componentId, const bool nativeComponentId)
    {
        ASSERT(world);

        cvar typeId = nativeComponentId ? componentId : ECS_MAX_NATIVE_COMPONENTS + componentId;
        ASSERT(typeId < ECS_MAX_COMPONENTS);

        cvar manager = world->GetEntityManager();
        ASSERT(manager->IsEntityValid(entityId));

        manager->RemoveEntityComponent(entityId, typeId);
    }

    void Activate(World* world, const EntityId entityId)
    {
        ASSERT(world);
        world->GetEntityManager()->Activate(entityId);
    }

    void Deactivate(World* world, const EntityId entityId)
    {
        ASSERT(world);
        world->GetEntityManager()->Deactivate(entityId);
    }

    void DestroyEntity(World* world, const EntityId entityId)
    {
        ASSERT(world);
        world->GetEntityManager()->Release(entityId);
    }

    void CleanEntity(World* world, const EntityId entityId)
    {
        ASSERT(world);
        world->GetEntityManager()->Clear(entityId);
    }

    Entity CreateEntity(World* world)
    {
        ASSERT(world);
        return world->CreateEntity();
    }
}

void Entity::InitRuntime()
{
    API_FILE("Common/Entities/EntityInternals.Gen.cs")
    {
        API_CLASS(INTERNAL, STATIC, "ReCrafted.API.Common.Entities", "EntityInternals");
        {
            API_METHOD(PUBLIC, STATIC, "ActivateEntity", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::ActivateEntity", &Internal::Activate);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "AddEntityComponent", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::AddEntityComponent", &Internal::AddComponent);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
                API_PARAM("IntPtr", "componentData");
                API_PARAM("uint", "componentSize");
                API_PARAM("ushort", "componentId");
                API_PARAM("bool", "nativeComponentId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "GetEntityComponent", EXTERN, UNSAFE);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::GetEntityComponent", &Internal::GetComponent);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
                API_PARAM("ushort", "componentId");
                API_PARAM("bool", "nativeComponentId");

                API_RETURN("ComponentData*")

            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "RemoveEntityComponent", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::RemoveEntityComponent", &Internal::RemoveComponent);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
                API_PARAM("ushort", "componentId");
                API_PARAM("bool", "nativeComponentId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "DeactivateEntity", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::DeactivateEntity", &Internal::Deactivate);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "DestroyEntity", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::DestroyEntity", &Internal::DestroyEntity);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "CleanEntity", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::CleanEntity", &Internal::CleanEntity);
                API_PARAM("IntPtr", "world");
                API_PARAM("uint", "entityId");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "CreateEntity", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entities.EntityInternals::CreateEntity", &Internal::CreateEntity);
                API_PARAM("IntPtr", "world");
                API_RETURN("Entity");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
