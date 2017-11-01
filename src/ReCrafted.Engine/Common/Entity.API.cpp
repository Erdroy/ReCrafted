// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Entity.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"
#include "EntityPool.h"


namespace Internal
{
    MonoObject* createEntity(MonoString* name)
    {
        auto nameString = MONO_TEXT(name);

        auto entity = EntityPool::createEntity(Text(nameString));

        Object::createInstance<Entity>(entity, "ReCrafted.API.Common", "Entity");
        
        return entity->getManagedPtr();
    }
}

void Entity::initRuntime()
{
    API_FILE("Common/Entity.Gen.cs")
    {
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
           
        }
        API_CLASS_END();
    }
    API_FILE_END();
}