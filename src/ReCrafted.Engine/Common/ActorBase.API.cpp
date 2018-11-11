// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorBase.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
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

    Transform* GetTransform(ActorBase* actor)
    {
        return &actor->GetTransform();
    }
}

void ActorBase::InitRuntime()
{
    API_FILE("Common/ActorBase.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");

        API_COMMENT("ActorBase class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "ActorBase", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(INTERNAL, REGULAR, "GetTransform", UNSAFE, NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetTransform", &Internal::GetTransform);
                API_RETURN("Transform*");
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

            API_METHOD(INTERNAL, REGULAR, "GetChildCount", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetChildCount", &Internal::GetChildCount);
                API_RETURN("int");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "GetChild", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Common.ActorBase::Internal_GetChild", &Internal::GetChild);
                API_PARAM("int", "index");
                API_RETURN("ActorBase");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
