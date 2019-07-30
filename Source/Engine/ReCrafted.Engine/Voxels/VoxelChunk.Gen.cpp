// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelChunk.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void VoxelChunk_Get_Visible1(VoxelChunk* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelChunk");
        const auto _returnValue = instance->GetVisible();
        *data = _returnValue;
    }

    static void VoxelChunk_Set_Visible1(VoxelChunk* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelChunk");
        instance->SetVisible(*data);
    }
    
    static Object* VoxelChunk_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'VoxelChunk' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<VoxelChunk>();
    }
};

void VoxelChunk::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::VoxelChunk_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(VoxelChunk);
    API_BIND("ReCrafted.API.Voxels.VoxelChunk::Get_InternalVisible", &APIProxy::VoxelChunk_Get_Visible1);
    API_BIND("ReCrafted.API.Voxels.VoxelChunk::Set_InternalVisible", &APIProxy::VoxelChunk_Set_Visible1);
}

const char* VoxelChunk::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelChunk";
}

const char* VoxelChunk::Name() 
{
    return "VoxelChunk";
}

const char* VoxelChunk::Namespace() 
{
    return "ReCrafted.API.Voxels";
}