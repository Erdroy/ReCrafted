// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "ActorBase.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:

    static void ActorBase_SetParent(ActorBase* instance, ActorBase* newParent) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetParent(newParent);
    }

    static void ActorBase_AddChild(ActorBase* instance, ActorBase* child) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->AddChild(child);
    }

    static void ActorBase_RemoveChild(ActorBase* instance, ActorBase* child) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->RemoveChild(child);
    }

    static void ActorBase_SetActive(ActorBase* instance, bool active) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetActive(active);
    }

    static void ActorBase_SetName(ActorBase* instance, MonoString* p_name) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_name);
        const auto name = MONO_STRING_TO_STR(p_name);
        ASSERT(instance);
        instance->SetName(name);
        MONO_FREE_STUB(name);
    }

    static MonoString* ActorBase_GetName(ActorBase* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetName();
        return MONO_STRING_FROM_STR(_returnValue);
    }
};

void ActorBase::InitRuntime() 
{
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetParent", &APIProxy::ActorBase_SetParent);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalAddChild", &APIProxy::ActorBase_AddChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalRemoveChild", &APIProxy::ActorBase_RemoveChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetActive", &APIProxy::ActorBase_SetActive);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetName", &APIProxy::ActorBase_SetName);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalGetName", &APIProxy::ActorBase_GetName);
}

const char* ActorBase::Fullname() 
{
    return "ReCrafted.API.Core.Actors.ActorBase";
}

const char* ActorBase::Name() 
{
    return "ActorBase";
}

const char* ActorBase::Namespace() 
{
    return "ReCrafted.API.Core.Actors";
}