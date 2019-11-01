// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelObjectAsset.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void VoxelObjectAsset_Get_ObjectName1(VoxelObjectAsset* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->ObjectName();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void VoxelObjectAsset_Get_ObjectType2(VoxelObjectAsset* instance, VoxelObjectType* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->ObjectType();
        *data = _returnValue;
    }
    
    static void VoxelObjectAsset_Get_MinimumSurfaceHeight3(VoxelObjectAsset* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->MinimumSurfaceHeight();
        *data = _returnValue;
    }
    
    static void VoxelObjectAsset_Get_MaximumSurfaceHeight4(VoxelObjectAsset* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->MaximumSurfaceHeight();
        *data = _returnValue;
    }
    
    static void VoxelObjectAsset_Get_HillsHeight5(VoxelObjectAsset* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->HillsHeight();
        *data = _returnValue;
    }
    
    static void VoxelObjectAsset_Get_InitialOctreeDepth6(VoxelObjectAsset* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _returnValue = instance->InitialOctreeDepth();
        *data = _returnValue;
    }
    
    static void VoxelObjectAsset_Get_VoxelObject7(VoxelObjectAsset* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectAsset");
        const auto _fRetValue = instance->VoxelObject();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static Object* VoxelObjectAsset_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'VoxelObjectAsset' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<VoxelObjectAsset>();
    }
};

void VoxelObjectAsset::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::VoxelObjectAsset_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(VoxelObjectAsset);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalObjectName", &APIProxy::VoxelObjectAsset_Get_ObjectName1);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalObjectType", &APIProxy::VoxelObjectAsset_Get_ObjectType2);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalMinimumSurfaceHeight", &APIProxy::VoxelObjectAsset_Get_MinimumSurfaceHeight3);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalMaximumSurfaceHeight", &APIProxy::VoxelObjectAsset_Get_MaximumSurfaceHeight4);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalHillsHeight", &APIProxy::VoxelObjectAsset_Get_HillsHeight5);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalInitialOctreeDepth", &APIProxy::VoxelObjectAsset_Get_InitialOctreeDepth6);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectAsset::Get_InternalVoxelObject", &APIProxy::VoxelObjectAsset_Get_VoxelObject7);
}

const char* VoxelObjectAsset::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelObjectAsset";
}

const char* VoxelObjectAsset::Name() 
{
    return "VoxelObjectAsset";
}

const char* VoxelObjectAsset::Namespace() 
{
    return "ReCrafted.API.Voxels";
}