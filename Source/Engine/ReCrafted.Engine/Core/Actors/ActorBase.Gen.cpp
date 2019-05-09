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
    
    static void ActorBase_Get_Parent(ActorBase* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _fRetValue = instance->GetParent();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static void ActorBase_Get_Position(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->Position();
        *data = _returnValue;
    }

    static void ActorBase_Set_Position(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->Position(*data);
    }
    
    static void ActorBase_Get_LocalPosition(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->LocalPosition();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalPosition(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->LocalPosition(*data);
    }
    
    static void ActorBase_Get_Rotation(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->Rotation();
        *data = _returnValue;
    }

    static void ActorBase_Set_Rotation(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->Rotation(*data);
    }
    
    static void ActorBase_Get_LocalRotation(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->LocalRotation();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalRotation(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->LocalRotation(*data);
    }
    
    static void ActorBase_Get_Scale(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->Scale();
        *data = _returnValue;
    }

    static void ActorBase_Set_Scale(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->Scale(*data);
    }
    
    static void ActorBase_Get_LocalScale(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->LocalScale();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalScale(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->LocalScale(*data);
    }
    
    static void ActorBase_Get_Transform(ActorBase* instance, Transform* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->GetTransform();
        *data = _returnValue;
    }

    static void ActorBase_Set_Transform(ActorBase* instance, Transform* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetTransform(*data);
    }
    
    static void ActorBase_Get_IsActiveSelf(ActorBase* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->IsActiveSelf();
        *data = _returnValue;
    }
    
    static void ActorBase_Get_IsActive(ActorBase* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->IsActive();
        *data = _returnValue;
    }
    
    static void ActorBase_Get_Name(ActorBase* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->GetName();
        *p_data = MONO_STRING_FROM_STR(_returnValue);
    }

    static void ActorBase_Set_Name(ActorBase* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_data);
        const auto data = MONO_STRING_TO_STR(*p_data);
        ASSERT(instance);
        instance->SetName(data);
        MONO_FREE_STUB(data);
    }
};

void ActorBase::InitRuntime() 
{
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetParent", &APIProxy::ActorBase_SetParent);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalAddChild", &APIProxy::ActorBase_AddChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalRemoveChild", &APIProxy::ActorBase_RemoveChild);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetActive", &APIProxy::ActorBase_SetActive);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalParent", &APIProxy::ActorBase_Get_Parent);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalPosition", &APIProxy::ActorBase_Get_Position);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalPosition", &APIProxy::ActorBase_Set_Position);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalPosition", &APIProxy::ActorBase_Get_LocalPosition);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalPosition", &APIProxy::ActorBase_Set_LocalPosition);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalRotation", &APIProxy::ActorBase_Get_Rotation);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalRotation", &APIProxy::ActorBase_Set_Rotation);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalRotation", &APIProxy::ActorBase_Get_LocalRotation);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalRotation", &APIProxy::ActorBase_Set_LocalRotation);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalScale", &APIProxy::ActorBase_Get_Scale);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalScale", &APIProxy::ActorBase_Set_Scale);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalScale", &APIProxy::ActorBase_Get_LocalScale);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalScale", &APIProxy::ActorBase_Set_LocalScale);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalTransform", &APIProxy::ActorBase_Get_Transform);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalTransform", &APIProxy::ActorBase_Set_Transform);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalIsActiveSelf", &APIProxy::ActorBase_Get_IsActiveSelf);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalIsActive", &APIProxy::ActorBase_Get_IsActive);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalName", &APIProxy::ActorBase_Get_Name);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalName", &APIProxy::ActorBase_Set_Name);
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