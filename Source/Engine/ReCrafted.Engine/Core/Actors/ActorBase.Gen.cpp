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
};

void ActorBase::InitRuntime() 
{
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetParent", &APIProxy::ActorBase_SetParent);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalAddChild", &APIProxy::ActorBase_AddChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalRemoveChild", &APIProxy::ActorBase_RemoveChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetActive", &APIProxy::ActorBase_SetActive);
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