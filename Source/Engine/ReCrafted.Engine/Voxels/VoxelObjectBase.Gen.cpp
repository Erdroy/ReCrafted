// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelObjectBase.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void VoxelObjectBase_Initialize1(VoxelObjectBase* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        instance->Initialize();
    }
    
    static void VoxelObjectBase_AddViewActor2(VoxelObjectBase* instance, ActorBase* viewActor) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        instance->AddViewActor(viewActor);
    }
    
    static void VoxelObjectBase_RemoveViewActor3(VoxelObjectBase* instance, ActorBase* viewActor) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        instance->RemoveViewActor(viewActor);
    }
    
    static void VoxelObjectBase_Modify4(VoxelObjectBase* instance, VoxelMaterial_t material, VoxelEditMode mode, const Vector3& position, float size) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        instance->Modify(material, mode, position, size);
    }
    
    static void VoxelObjectBase_Get_Radius1(VoxelObjectBase* instance, double* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        const auto _returnValue = instance->Radius();
        *data = _returnValue;
    }
    
    static void VoxelObjectBase_Get_Asset2(VoxelObjectBase* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        const auto _fRetValue = instance->Asset();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static void VoxelObjectBase_Get_RigidBody3(VoxelObjectBase* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        const auto _fRetValue = instance->RigidBody();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static void VoxelObjectBase_Get_IsLoading4(VoxelObjectBase* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        const auto _returnValue = instance->IsLoading();
        *data = _returnValue;
    }
};

void VoxelObjectBase::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(VoxelObjectBase);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::InternalInitialize", &APIProxy::VoxelObjectBase_Initialize1);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::InternalAddViewActor", &APIProxy::VoxelObjectBase_AddViewActor2);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::InternalRemoveViewActor", &APIProxy::VoxelObjectBase_RemoveViewActor3);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::InternalModify", &APIProxy::VoxelObjectBase_Modify4);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::Get_InternalRadius", &APIProxy::VoxelObjectBase_Get_Radius1);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::Get_InternalAsset", &APIProxy::VoxelObjectBase_Get_Asset2);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::Get_InternalRigidBody", &APIProxy::VoxelObjectBase_Get_RigidBody3);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::Get_InternalIsLoading", &APIProxy::VoxelObjectBase_Get_IsLoading4);
}

const char* VoxelObjectBase::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelObjectBase";
}

const char* VoxelObjectBase::Name() 
{
    return "VoxelObjectBase";
}

const char* VoxelObjectBase::Namespace() 
{
    return "ReCrafted.API.Voxels";
}