// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Entity.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Mono.h"

void Entity::initRuntime()
{
    API_FILE("Common/Entity.Gen.cs")
    {
        API_COMMENT("Entity class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Entity");
        {
           
        }
        API_CLASS_END();
    }
    API_FILE_END();
}