// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"

void Entity::InitRuntime()
{
    API_FILE("Common/Entities/Entity.Gen.cs")
    {
        API_COMMENT("ECS Entity class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common.Entities", "Entity", "", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
