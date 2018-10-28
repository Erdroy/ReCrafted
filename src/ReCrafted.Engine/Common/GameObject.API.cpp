// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObject.h"
#include "Scripting/ScriptingAPI.h"
#include "GameObjectPool.h"

namespace Internal
{
    MonoObject* GetChild(GameObject* gameObject, const int index)
    {
        ASSERT(gameObject);

        if (index < 0)
            return nullptr;

        crvar childrenArray = gameObject->Children();

        if (index >= static_cast<int>(childrenArray.Count()))
            return nullptr;

        return childrenArray[index]->GetManagedPtr();
    }

    int GetChildCount(GameObject* gameObject)
    {
        ASSERT(gameObject);
        crvar childrenArray = gameObject->Children();
        return childrenArray.Count();
    }

    void AddChildren(GameObject* gameObject, GameObject* children)
    {
        ASSERT(gameObject);
        ASSERT(children);

        gameObject->AddChildren(children);
    }

    void RemoveChildren(GameObject* gameObject, GameObject* children)
    {
        ASSERT(gameObject);
        ASSERT(children);

        gameObject->RemoveChildren(children);
    }

    void AddScript(GameObject* gameObject, Script* script)
    {
        DEBUG_ASSERT(gameObject);
        DEBUG_ASSERT(script);
        DEBUG_ASSERT(script->GetGameObject() == nullptr);

        gameObject->AddScript(script);
    }

    void RemoveScript(GameObject* gameObject, Script* script)
    {
        DEBUG_ASSERT(gameObject);
        DEBUG_ASSERT(script);

        gameObject->RemoveScript(script);
    }

    TransformComponent* GetTransform(GameObject* gameObject)
    {
        return gameObject->GetTransform();
    }

    Object* CreateGameObject(bool createManagedInstance)
    {
        return GameObject::Create();
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
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AddScript", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_AddScript", &Internal::AddScript);
                API_PARAM("IntPtr", "nativeScriptPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "RemoveScript", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_RemoveScript", &Internal::RemoveScript);
                API_PARAM("IntPtr", "nativeScriptPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AddChildren", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_AddChildren", &Internal::AddChildren);
                API_PARAM("IntPtr", "gameObjectPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "RemoveChildren", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_RemoveChildren", &Internal::RemoveChildren);
                API_PARAM("IntPtr", "gameObjectPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "GetChildCount", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_GetChildCount", &Internal::GetChildCount);
                API_RETURN("int")
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "GetChild", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.GameObject::Internal_GetChild", &Internal::GetChild);
                API_PARAM("int", "index")
                API_RETURN("GameObject")
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
