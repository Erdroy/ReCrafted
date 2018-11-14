// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorBase.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    void GetPosition(ActorBase* actor, Vector3* position)
    {
        ASSERT(actor);
        *position = actor->GetPosition();
    }

    void SetPosition(ActorBase* actor, Vector3* position)
    {
        ASSERT(actor);
        actor->SetPosition(*position);
    }

    void GetLocalPosition(ActorBase* actor, Vector3* position)
    {
        ASSERT(actor);
        *position = actor->GetLocalPosition();
    }

    void SetLocalPosition(ActorBase* actor, Vector3* position)
    {
        ASSERT(actor);
        actor->SetLocalPosition(*position);
    }

    void GetRotation(ActorBase* actor, Quaternion* rotation)
    {
        ASSERT(actor);
        *rotation = actor->GetRotation();
    }

    void SetRotation(ActorBase* actor, Quaternion* rotation)
    {
        ASSERT(actor);
        actor->SetRotation(*rotation);
    }

    void GetLocalRotation(ActorBase* actor, Quaternion* rotation)
    {
        ASSERT(actor);
        *rotation = actor->GetLocalRotation();
    }

    void SetLocalRotation(ActorBase* actor, Quaternion* rotation)
    {
        ASSERT(actor);
        actor->SetLocalRotation(*rotation);
    }

    void GetScale(ActorBase* actor, Vector3* scale)
    {
        ASSERT(actor);
        *scale = actor->GetScale();
    }

    void SetScale(ActorBase* actor, Vector3* scale)
    {
        ASSERT(actor);
        actor->SetScale(*scale);
    }

    void GetLocalScale(ActorBase* actor, Vector3* scale)
    {
        ASSERT(actor);
        *scale = actor->GetLocalScale();
    }

    void SetLocalScale(ActorBase* actor, Vector3* scale)
    {
        ASSERT(actor);
        actor->SetLocalScale(*scale);
    }

    MonoObject* GetChild(ActorBase* actor, const int index)
    {
        ASSERT(actor);

        if (index < 0)
            return nullptr;

        crvar childrenArray = actor->GetChildren();

        if (index >= static_cast<int>(childrenArray.Count()))
            return nullptr;

        return childrenArray[index]->GetManagedPtr();
    }

    void Destroy(ActorBase* actor)
    {
        ASSERT(actor);
        actor->Destroy();
    }

    void SetIsActive(ActorBase* actor, const bool active)
    {
        ASSERT(actor);
        
        actor->SetActive(active);
    }

    bool GetIsActive(ActorBase* actor)
    {
        ASSERT(actor);

        return actor->IsActive();
    }

    bool GetIsActiveSelf(ActorBase* actor)
    {
        ASSERT(actor);

        return actor->IsActiveSelf();
    }

    int GetChildCount(ActorBase* actor)
    {
        ASSERT(actor);
        crvar childrenArray = actor->GetChildren();
        return childrenArray.Count();
    }

    void SetParent(ActorBase* actor, ActorBase* parent)
    {
        ASSERT(actor);
        ASSERT(parent);

        actor->SetParent(parent);
    }

    void AddChildren(ActorBase* actor, ActorBase* children)
    {
        ASSERT(actor);
        ASSERT(children);

        actor->AddChild(children);
    }

    void RemoveChildren(ActorBase* actor, ActorBase* children)
    {
        ASSERT(actor);
        ASSERT(children);

        actor->RemoveChild(children);
    }

    void AddScript(ActorBase* actor, Script* script)
    {
        DEBUG_ASSERT(actor);
        DEBUG_ASSERT(script);
        DEBUG_ASSERT(script->GetActor() == nullptr);

        actor->AddScript(script);
    }

    void RemoveScript(ActorBase* actor, Script* script)
    {
        DEBUG_ASSERT(actor);
        DEBUG_ASSERT(script);

        actor->RemoveScript(script);
    }

    Transform GetTransform(ActorBase* actor)
    {
        return actor->GetTransform();
    }

    void SetTransform(ActorBase* actor, Transform* transform)
    {
        return actor->SetTransform(*transform);
    }
}

void ActorBase::InitRuntime()
{
    API_FILE("Common/ActorBase.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("ActorBase class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "ActorBase", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            /* Transform */

            API_METHOD(INTERNAL, REGULAR, "GetTransform", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetTransform", &Internal::GetTransform);
                API_RETURN("Transform");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "SetTransform", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_SetTransform", &Internal::SetTransform);
                API_PARAM("ref Transform", "transform");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets current actor position.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Position", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Position_Get", &Internal::GetPosition);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Position_Set", &Internal::SetPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current actor position.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "LocalPosition", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalPosition_Get", &Internal::GetLocalPosition);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalPosition_Set", &Internal::SetLocalPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current actor rotation.");
            API_PROPERTY(PUBLIC, REGULAR, "Quaternion", "Rotation", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Rotation_Get", &Internal::GetRotation);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Rotation_Set", &Internal::SetRotation);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current actor rotation.");
            API_PROPERTY(PUBLIC, REGULAR, "Quaternion", "LocalRotation", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalRotation_Get", &Internal::GetLocalRotation);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalRotation_Set", &Internal::SetLocalRotation);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current actor scale.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Scale", GETSET);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Scale_Get", &Internal::GetScale);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Scale_Set", &Internal::SetScale);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current actor scale.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "LocalScale", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalScale_Get", &Internal::GetLocalScale);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_LocalScale_Set", &Internal::SetLocalScale);
            }
            API_PROPERTY_END();


            /* Basics */

            API_COMMENT("Gets children count of this actor.");
            API_PROPERTY(PUBLIC, REGULAR, "int", "ChildCount", GET);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_ChildCount_Get", &Internal::GetChildCount);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets child from this actor with given index.");
            API_METHOD(PUBLIC, REGULAR, "GetChild", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetChild", &Internal::GetChild);
                API_PARAM("int", "index");
                API_RETURN("ActorBase");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AddScript", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_AddScript", &Internal::AddScript);
                API_PARAM("IntPtr", "nativeScriptPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "RemoveScript", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_RemoveScript", &Internal::RemoveScript);
                API_PARAM("IntPtr", "nativeScriptPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "SetParent", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_SetParent", &Internal::SetParent);
                API_PARAM("IntPtr", "actorPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AddChild", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_AddChild", &Internal::AddChildren);
                API_PARAM("IntPtr", "actorPtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "RemoveChild", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_RemoveChild", &Internal::RemoveChildren);
                API_PARAM("IntPtr", "actorPtr");
            }
            API_METHOD_END();

            API_COMMENT("Destroys this actor. Actor will be released at the end of the current frame.");
            API_METHOD(PUBLIC, REGULAR_NEW, "Destroy", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_Destroy", &Internal::Destroy);
            }
            API_METHOD_END();

            API_COMMENT("Activates or deactivates this actor.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsActive", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_IsActive_Get", &Internal::GetIsActive);
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_IsActive_Set", &Internal::SetIsActive);
            }
            API_PROPERTY_END();

            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsActiveSelf", GET);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetIsActiveSelf", &Internal::GetIsActiveSelf);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
