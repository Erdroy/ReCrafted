// ReCrafted © 2016-2017 Always Too Late

#include "Entity.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Mono.h"
#include "EntityPool.h"


namespace Internal
{
    MonoObject* createEntity(MonoString* name)
    {
        auto entity = EntityPool::createEntity(TEXT_FROM_MONO_STRING(name));

        Object::createInstance<Entity>(entity, "ReCrafted.API.Common", "Entity");
        
        return entity->getManagedPtr();
    }

    Vector3 getPosition(Entity* entity)
    {
        if (!entity)
            return Vector3::zero();

        return entity->position;
    }

    Vector3 getRotation(Entity* entity)
    {
        if (!entity)
            return Vector3::zero();

        return entity->rotation;
    }

    void setPosition(Entity* entity, Vector3 position)
    {
        if (!entity)
            return;

        entity->position = position;
    }

    void setRotation(Entity* entity, Vector3 rotation)
    {
        if (!entity)
            return;

        entity->rotation = rotation;
    }

    Guid getGuid(Entity* entity)
    {
        if (!entity)
            return {};

        return entity->guid;
    }

    MonoString* getName(Entity* entity)
    {
        if (!entity)
            return nullptr;

        return MONO_STRING_FROM_TEXT(entity->name);
    }

    void setName(Entity* entity, MonoString* name)
    {
        if (!entity)
            return;

        auto newName = Text(MONO_TEXT(name));
        entity->name = newName;
    }
}

void Entity::initRuntime()
{
    API_FILE("Common/Entity.Gen.cs")
    {
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Entity class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Entity", "Object");
        {
            API_COMMENT("Creates new Entity");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Entity::Create", &Internal::createEntity);

                API_PARAM("string", "entityName");

                API_RETURN("Entity");
            }
            API_METHOD_END();

            API_COMMENT("The position of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Position", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Position_Get", &Internal::getPosition);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Position_Set", &Internal::setPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("The rotation of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Rotation", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Rotation_Get", &Internal::getRotation);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Rotation_Set", &Internal::setRotation);
            }
            API_PROPERTY_END();

            API_COMMENT("The guid of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "Guid", "Guid", GET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Guid_Get", &Internal::getGuid);
            }
            API_PROPERTY_END();

            API_COMMENT("The name of this entity.");
            API_PROPERTY(PUBLIC, REGULAR, "string", "Name", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Entity::Internal_Name_Get", &Internal::getName);
                API_BIND("ReCrafted.API.Common.Entity::Internal_Name_Set", &Internal::setName);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}