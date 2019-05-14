// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "ActorBase.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void ActorBase_SetParent1(ActorBase* instance, ActorBase* newParent) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->SetParent(newParent);
    }
    
    static void ActorBase_AddChild2(ActorBase* instance, ActorBase* child) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->AddChild(child);
    }
    
    static void ActorBase_RemoveChild3(ActorBase* instance, ActorBase* child) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->RemoveChild(child);
    }
    
    static void ActorBase_AddScript4(ActorBase* instance, Script* script) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->AddScript(script);
    }
    
    static void ActorBase_RemoveScript5(ActorBase* instance, Script* script) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->RemoveScript(script);
    }
    
    static void ActorBase_SetActive6(ActorBase* instance, bool active) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->SetActive(active);
    }
    
    static void ActorBase_Get_Parent1(ActorBase* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _fRetValue = instance->GetParent();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static void ActorBase_Get_Position2(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->Position();
        *data = _returnValue;
    }

    static void ActorBase_Set_Position2(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->Position(*data);
    }
    
    static void ActorBase_Get_LocalPosition3(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->LocalPosition();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalPosition3(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->LocalPosition(*data);
    }
    
    static void ActorBase_Get_Rotation4(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->Rotation();
        *data = _returnValue;
    }

    static void ActorBase_Set_Rotation4(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->Rotation(*data);
    }
    
    static void ActorBase_Get_LocalRotation5(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->LocalRotation();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalRotation5(ActorBase* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->LocalRotation(*data);
    }
    
    static void ActorBase_Get_Scale6(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->Scale();
        *data = _returnValue;
    }

    static void ActorBase_Set_Scale6(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->Scale(*data);
    }
    
    static void ActorBase_Get_LocalScale7(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->LocalScale();
        *data = _returnValue;
    }

    static void ActorBase_Set_LocalScale7(ActorBase* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->LocalScale(*data);
    }
    
    static void ActorBase_Get_Transform8(ActorBase* instance, Transform* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->GetTransform();
        *data = _returnValue;
    }

    static void ActorBase_Set_Transform8(ActorBase* instance, Transform* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->SetTransform(*data);
    }
    
    static void ActorBase_Get_IsActiveSelf9(ActorBase* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->IsActiveSelf();
        *data = _returnValue;
    }
    
    static void ActorBase_Get_IsActive10(ActorBase* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->IsActive();
        *data = _returnValue;
    }
    
    static void ActorBase_Get_Name11(ActorBase* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->GetName();
        *p_data = MONO_STRING_FROM_STR(_returnValue);
    }

    static void ActorBase_Set_Name11(ActorBase* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_data);
        const auto data = MONO_STRING_TO_STR(*p_data);
        MONO_CHECK_OBJECT(instance, "ActorBase");
        instance->SetName(data);
        MONO_FREE_STUB(data);
    }
    
    static void ActorBase_Get_Children12(ActorBase* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->GetChildren();
        *p_data = MONO_ARRAY_FROM_OBJECT_ARRAY(_returnValue, ActorBase, ActorBase*, MonoObject*, _t0->ToManaged());
    }
    
    static void ActorBase_Get_TestArr13(ActorBase* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->TestArr();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, int, int, int, _t0);
    }
    
    static void ActorBase_Get_Scripts14(ActorBase* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "ActorBase");
        const auto _returnValue = instance->GetScripts();
        *p_data = MONO_ARRAY_FROM_OBJECT_ARRAY(_returnValue, Script, Script*, MonoObject*, _t0->ToManaged());
    }
};

void ActorBase::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(ActorBase);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetParent", &APIProxy::ActorBase_SetParent1);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalAddChild", &APIProxy::ActorBase_AddChild2);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalRemoveChild", &APIProxy::ActorBase_RemoveChild3);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalAddScript", &APIProxy::ActorBase_AddScript4);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalRemoveScript", &APIProxy::ActorBase_RemoveScript5);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::InternalSetActive", &APIProxy::ActorBase_SetActive6);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalParent", &APIProxy::ActorBase_Get_Parent1);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalPosition", &APIProxy::ActorBase_Get_Position2);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalPosition", &APIProxy::ActorBase_Set_Position2);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalPosition", &APIProxy::ActorBase_Get_LocalPosition3);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalPosition", &APIProxy::ActorBase_Set_LocalPosition3);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalRotation", &APIProxy::ActorBase_Get_Rotation4);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalRotation", &APIProxy::ActorBase_Set_Rotation4);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalRotation", &APIProxy::ActorBase_Get_LocalRotation5);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalRotation", &APIProxy::ActorBase_Set_LocalRotation5);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalScale", &APIProxy::ActorBase_Get_Scale6);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalScale", &APIProxy::ActorBase_Set_Scale6);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalLocalScale", &APIProxy::ActorBase_Get_LocalScale7);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalLocalScale", &APIProxy::ActorBase_Set_LocalScale7);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalTransform", &APIProxy::ActorBase_Get_Transform8);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalTransform", &APIProxy::ActorBase_Set_Transform8);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalIsActiveSelf", &APIProxy::ActorBase_Get_IsActiveSelf9);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalIsActive", &APIProxy::ActorBase_Get_IsActive10);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalName", &APIProxy::ActorBase_Get_Name11);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Set_InternalName", &APIProxy::ActorBase_Set_Name11);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalChildren", &APIProxy::ActorBase_Get_Children12);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalTestArr", &APIProxy::ActorBase_Get_TestArr13);
    API_BIND("ReCrafted.API.Core.Actors.ActorBase::Get_InternalScripts", &APIProxy::ActorBase_Get_Scripts14);
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