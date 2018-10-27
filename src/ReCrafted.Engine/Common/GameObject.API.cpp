// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObject.h"
#include "Scripting/ScriptingAPI.h"
#include "GameObjectPool.h"

namespace Internal
{
    TransformComponent* GetTransform(GameObject* gameObject)
    {
        return gameObject->GetTransform();
    }

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
        API_USING("ReCrafted.API.Common.Components");

        API_COMMENT("GameObject class. Every game object has it's own ECS entity, by default assigned with TransformComponent.");
        API_CLASS(PUBLIC, SEALED, "ReCrafted.API.Common", "GameObject", "Object", PARTIAL);
        {
            API_METHOD(INTERNAL, REGULAR, "GetTransform", UNSAFE, NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::GetTransform", &Internal::GetTransform);
                API_RETURN("TransformComponent*");
            }

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
