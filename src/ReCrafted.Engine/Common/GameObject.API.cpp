// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObject.h"
#include "Scripting/ScriptingAPI.h"
#include "GameObjectPool.h"

namespace Internal
{
    Object* CreateGameObject()
    {
        return GameObjectPool::AcquireGameObject();
    }
}

void GameObject::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Common.GameObject", &Internal::CreateGameObject);

    API_FILE("Common/GameObject.Gen.cs");
    {
        API_COMMENT("GameObject class.");
        API_CLASS(PUBLIC, SEALED, "ReCrafted.API.Common", "GameObject", "Object", PARTIAL, NOCONSTRUCTOR);
        {
           
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
