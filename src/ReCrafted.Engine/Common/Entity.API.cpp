// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"
#include "Scripting/Mono.h"
#include "EntityPool.h"


namespace Internal
{
    MonoObject* CreateEntity(MonoString* name)
    {
        auto entity = EntityPool::CreateEntity(TEXT_FROM_MONO_STRING(name));

        Object::CreateInstance<Entity>(entity, "ReCrafted.API.Common", "Entity");

        return entity->GetManagedPtr();
    }

    Vector3 GetPosition(Entity* entity)
    {
        if (!entity)
            return Vector3::Zero();

        return entity->position;
    }

    Vector3 GetRotation(Entity* entity)
    {
        if (!entity)
            return Vector3::Zero();

        return entity->rotation;
    }

    void SetPosition(Entity* entity, Vector3 position)
    {
        if (!entity)
            return;

        entity->position = position;
    }

    void SetRotation(Entity* entity, Vector3 rotation)
    {
        if (!entity)
            return;

        entity->rotation = rotation;
    }

    Guid GetGuid(Entity* entity)
    {
        if (!entity)
            return {};

        return entity->guid;
    }

    MonoString* GetName(Entity* entity)
    {
        if (!entity)
            return nullptr;

        return MONO_STRING_FROM_TEXT(entity->name);
    }

    void SetName(Entity* entity, MonoString* name)
    {
        if (!entity)
            return;

        auto newName = Text(MONO_TEXT(name));
        entity->name = newName;
    }

    void AddScript(Entity* entity, MonoObject* scriptInstance)
    {
        RefPtr<Script> script(new Script);

        Object::InitializeInstance(script, scriptInstance);
        entity->AddScript(script);

        script->Init(entity);
    }

    void RemoveScript(Entity* entity, Script* script)
    {
        // TODO: Remove scripts
    }
}

void Entity::InitRuntime()
{
    API_FILE("Common/Entity.Gen.cs")
    {
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Entity class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Entity", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Creates new Entity");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entity::Create", &Internal::CreateEntity);

                API_PARAM("string", "entityName");

                API_RETURN("Entity");
            }
            API_METHOD_END();

            API_COMMENT("The position of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Position", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Position_Get", &Internal::GetPosition);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Position_Set", &Internal::SetPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("The rotation of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Rotation", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Rotation_Get", &Internal::GetRotation);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Rotation_Set", &Internal::SetRotation);
            }
            API_PROPERTY_END();

            API_COMMENT("The guid of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Guid", "Guid", GET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Guid_Get", &Internal::GetGuid);
            }
            API_PROPERTY_END();

            API_COMMENT("The name of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "string", "Name", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Name_Get", &Internal::GetName);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Name_Set", &Internal::SetName);
            }
            API_PROPERTY_END();

            API_METHOD(PRIVATE, REGULAR, "AddScript", EXTERN, NOPROXY);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_AddScript", &Internal::AddScript);
                API_PARAM("Script", "instance");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, REGULAR, "RemoveScript", EXTERN, NOPROXY);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_RemoveScript", &Internal::RemoveScript);
                API_PARAM("Script", "instance");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
