// ReCrafted (c) 2016-2018 Always Too Late

#include "Entity.h"

void Entity::InitRuntime()
{
    API_FILE("Common/Entities/EntityInternals.Gen.cs")
    {
        API_CLASS(INTERNAL, STATIC, "ReCrafted.API.Common.Entities", "EntityInternals");
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
